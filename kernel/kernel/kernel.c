// If I manage to make the disk reader work, mention Salvador Garcia and Bill Pierce in my thanks.

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/tty.h>

#include <kernel/cmos.h>
#include <kernel/datetime.h>
#include <kernel/keyboard.h>
#include <kernel/timer.h>
#include <kernel/harddrive.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/fpu.h>
#include <kernel/speaker.h>
#include <kernel/util.h>

#include <kernel/parser.h>

bool parse_cls(char* line) {
    char** linePtr = &line;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "cls")) {
        uint8_t color = terminal_get_cursor_color();
        int bgColor = color >> 4;
        int fgColor = color & 0x0F;

        skip_whitespace(linePtr);
        if (read_integer(linePtr, &bgColor)) {
            //printf("Found background color: %d\r\n", bgColor);
        } else {
            //printf("No background color specified.\r\n");
        }

        skip_whitespace(linePtr);
        if (match_char(linePtr, ',')) {
            //printf("Found comma.\r\n");

            skip_whitespace(linePtr);
            if (read_integer(linePtr, &fgColor)) {
                //printf("Found foreground color: %d\r\n", fgColor);

                skip_whitespace(linePtr);
                if (!is_at_eol(linePtr)) {
                    // Expected EOL here.
                    printf("?SN ERROR\r\n");
                    return false;
                }
            } else {
                // If you put the comma, you gotta put the number.
                printf("?SN ERROR\r\n");
                return false;
            }
        } else if (!is_at_eol(linePtr)) {
            //printf("Remainder: '%s'\r\n", *linePtr);
            printf("?SN ERROR\r\n");
            return false;
        }

        if ((bgColor < 0) || (bgColor > 15) || (fgColor < 0) || (fgColor > 15)) {
            printf("?FC ERROR\r\n");
            return false;
        }

        terminal_clear(vga_entry_color(fgColor, bgColor));
        return true;
    }
    return false;
}

const char* drive_get_description(uint8_t driveDesc) {
    switch (driveDesc) {
        case 0:
            return "NOT FOUND";
        case 1:
            return "360 KB 5.25 DRIVE";
        case 2:
            return "1.2 MB 5.25 DRIVE";
        case 3:
            return "720 KB 3.5 DRIVE";
        case 4:
            return "1.44 MB 3.5 DRIVE";
        case 5:
            return "2.88 MB 3.5 DRIVE";
        default:
            return "DRIVE TYPE UNKNOWN";
    }
}

bool parse_drives(char* line) {
    char** linePtr = &line;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "drives")) {
        skip_whitespace(linePtr);
        if (!is_at_eol(linePtr)) {
            // Expected EOL here.
            printf("?SN ERROR\r\n");
            return false;
        }

        // Read the number of drives from CMOS register 0x10.
        cmos_select_register(0x10);
        
        uint8_t driveDesc = cmos_read();
        uint8_t drive0Desc = driveDesc >> 4;
        uint8_t drive1Desc = driveDesc & 0x0F;
        printf("DRIVE 0: %s\r\n", drive_get_description(drive0Desc));
        printf("DRIVE 1: %s\r\n", drive_get_description(drive1Desc));
        return true;
    }
    return false;
}

bool parse_date(char* line) {
    char** linePtr = &line;
    bool displayTime = false;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "date")) {
        skip_whitespace(linePtr);

        if (match_char(linePtr, '-') && match_ident(linePtr, "t")) {
            // Display the time as well.
            displayTime = true;
        }
        skip_whitespace(linePtr);

        if (!is_at_eol(linePtr)) {
            printf("?SN ERROR\r\n");
            return false;
        }
        
        datetime_read_rtc();
        printf("%04d-%02d-%02d", datetime_year, datetime_month, datetime_day);

        if (displayTime) {
            printf(" %02d:%02d:%02d", datetime_hour, datetime_minute, datetime_second);
        }
        printf("\r\n");
        return true;
    }
    return false;
}

bool parse_beep(char* line) {
    char** linePtr = &line;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "beep")) {
        skip_whitespace(linePtr);

        if (!is_at_eol(linePtr)) {
            printf("?SN ERROR\r\n");
            return false;
        }
            
        speaker_play(1000);
        timer_sleep(10);
        speaker_pause();
        
        return true;
    }
    return false;
}

bool parse_sound(char* line) {
    char** linePtr = &line;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "sound")) {
        int frequency;
        int duration;

        skip_whitespace(linePtr);
        if (!read_integer(linePtr, &frequency)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        skip_whitespace(linePtr);
        if (!match_char(linePtr, ',')) {
            printf("?SN ERROR\r\n");
            return false;
        }

        skip_whitespace(linePtr);
        if (!read_integer(linePtr, &duration)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        skip_whitespace(linePtr);
        if (!is_at_eol(linePtr)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        speaker_play(frequency);
        timer_sleep(duration);
        speaker_pause();
        return true;
    }
    return false;
}


#define REG_CYL_LO 4
#define REG_CYL_HI 5
#define REG_DEVSEL 6
void hd_read_test(uint32_t LBA) {
    uint8_t slavebit = 0;
    uint8_t sectorCount = 1;
    outportb(0x1F6, 0xE0 | (slavebit << 4) | ((LBA >> 24) & 0x0F));
    outportb(0x1F1, 0x00);
    outportb(0x1F2, sectorCount);
    outportb(0x1F3, (uint8_t)(LBA & 0xFF));
    outportb(0x1F4, (uint8_t)((LBA >> 8) & 0xFF));
    outportb(0x1F5, (uint8_t)((LBA >> 16) & 0xFF));
    outportb(0x1F7, 0x20);

    while ((inportb(0x1F7) & 0x8) == 0) {
        // Wait until bit 3 is set?
    }

    for (int n = 0; n < 256; n++) {
        uint16_t value = inportw(0x1F0);
        printf("%c%c", value & 0xFF, (value >> 8));
        //if ((n % 20) == 0) {
        //    printf("\r\n");
        //}
    }
}

bool parse_loadm(char* line) {
    char** linePtr = &line;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "loadm")) {
        int sector;

        skip_whitespace(linePtr);
        if (!read_integer(linePtr, &sector)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        skip_whitespace(linePtr);
        if (!is_at_eol(linePtr)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        hd_read_test(sector);

        return true;
    }
    return false;
}

bool parse_dir(char* line) {
    char** linePtr = &line;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "dir")) {
        skip_whitespace(linePtr);
        if (!is_at_eol(linePtr)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        uint8_t* data = harddrive_load_sector(154);
        int n = 0;
        while (true) {
            if ((data[n] == 255) || (n >= 512)) {
                break;
            }
            
            // Print the filename.
            for (int m = 0; m < 8; m++) {
                printf("%c", data[n + m]);
            }
            n += 8;

            printf(" ");

            // Print the extension.
            for (int m = 0; m < 3; m++) {
                printf("%c", data[n + m]);
            }
            n += 3;

            printf(" ");

            // Print the file type.
            printf("%c", data[n] + '0');
            n++;

            printf(" ");

            // Print the format type.
            printf("%c", (data[n] == 0) ? 'B' : 'A');
            n++;

            printf(" ");

            // Print the first granule location.
            printf("%d", data[n]);
            n++;

            printf(" ");

            // Print the last bytes size.
            int lastBytes = data[n] + (data[n + 1] << 8);
            printf("%d\r\n", lastBytes);
            n++;

            // Skip the final 16 bytes;
            n += 17;
        }
        free(data);

        return true;
    }
    return false;
}

bool parse_width(char* line) {
    char** linePtr = &line;
    
    skip_whitespace(linePtr);
    if (match_ident(linePtr, "width")) {
        int width;

        skip_whitespace(linePtr);
        if (!read_integer(linePtr, &width)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        if ((width != 40) && (width != 80)) {
            printf("?FC ERROR\r\n");
            return false;
        }

        skip_whitespace(linePtr);
        if (!is_at_eol(linePtr)) {
            printf("?SN ERROR\r\n");
            return false;
        }

        if (width == 40) {
            vga_set_mode_text_40x25();
        } else if (width == 80) {
            vga_set_mode_text_80x25();
        }
        terminal_clear(terminal_get_cursor_color());

        return true;
    }
    return false;
}

__attribute__ ((constructor)) void kernel_premain(void) {
    terminal_clear(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_GREEN));
	//printf("The terminal is initialized.\r\n");

    // Make a blocky cursor.
    //cursor_enable(0, 15);
    cursor_disable();

    idt_init();
    //printf("IDF is initialized.\r\n");

    isr_init();
    //printf("ISR is initialized.\r\n");

    fpu_init();
    //printf("FPU is initialized.\r\n");

    irq_init();
    //printf("IRQ is initialized.\r\n");

    timer_init();
    //printf("Timer is initialized.\r\n");

    keyboard_init();
    //printf("Keyboard is initialized.\r\n");

    //floppy_init();

    vga_set_mode_text_40x25();
    
	printf("X32 OVER-EXTENDED DISK COLOR BASIC 0.1.2A\r\n");
    printf("COPR. 2021 BY TREY TOMES\r\n");
    printf("\r\n");
}

void kernel_main(void) {
    printf("PRIMARY HD: ");
    uint8_t type = harddrive_detect_devtype(0, 0x1F0, 0x3F6);
    switch (type) {
        case ATADEV_PATAPI:
            printf("PATAPI");
            break;
        case ATADEV_SATAPI:
            printf("SATAPI");
            break;
        case ATADEV_PATA:
            printf("PATA");
            break;
        case ATADEV_SATA:
            printf("SATA");
            break;
        case ATADEV_UNKNOWN:
        default:
            printf("UNKNOWN");
            break;
    }
    printf("\r\n");
    printf("SECONDARY HD: ");
    type = harddrive_detect_devtype(1, 0x1F0, 0x3F6);
    switch (type) {
        case ATADEV_PATAPI:
            printf("PATAPI");
            break;
        case ATADEV_SATAPI:
            printf("SATAPI");
            break;
        case ATADEV_PATA:
            printf("PATA");
            break;
        case ATADEV_SATA:
            printf("SATA");
            break;
        case ATADEV_UNKNOWN:
        default:
            printf("UNKNOWN");
            break;
    }
    printf("\r\n");

    size_t line_length = vga_width;
    char* line = (char*)malloc(vga_width);

    printf("OK\r\n");

    while (true) {
        size_t len = getline(&line, &line_length);

        if (len > 0) {
            if (starts_with(line, "beep")) {
                parse_beep(line);
            } if (starts_with(line, "cls")) {
                parse_cls(line);
            } else if (starts_with(line, "date")) {
                parse_date(line);
            } else if (starts_with(line, "dir")) {
                parse_dir(line);
            } else if (starts_with(line, "drives")) {
                parse_drives(line);
            } else if (starts_with(line, "loadm")) {
                parse_loadm(line);
            } else if (starts_with(line, "sound")) {
                parse_sound(line);
            } else if (starts_with(line, "width")) {
                parse_width(line);
            } else {
                //char text[32] = {0};
                //itoa(len, text, 32);
                printf("You said: %s (Length: %d)\r\n", line, len);
            }
            printf("OK\r\n");
        }
    }
}
