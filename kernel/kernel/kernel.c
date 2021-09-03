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
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/fpu.h>

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

	printf("X32 OVER-EXTENDED COLOR BASIC 0.1.1A\r\n");
    printf("COPR. 2021 BY TREY TOMES\r\n");
    printf("\r\n");
}

void kernel_main(void) {
    size_t line_length = VGA_WIDTH;
    char* line = (char*)malloc(VGA_WIDTH);

    printf("OK\r\n");

    while (true) {
        size_t len = getline(&line, &line_length);

        if (len > 0) {
            if (starts_with(line, "cls")) {
                parse_cls(line);
            } else if (starts_with(line, "date")) {
                parse_date(line);
            } else if (starts_with(line, "drives")) {
                parse_drives(line);
            } else {
                //char text[32] = {0};
                //itoa(len, text, 32);
                printf("You said: %s (Length: %d)\r\n", line, len);
            }
            printf("OK\r\n");
        }
    }
}
