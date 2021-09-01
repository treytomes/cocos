#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/tty.h>

#include <kernel/keyboard.h>
#include <kernel/timer.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/fpu.h>

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

	printf("X32 OVER-EXTENDED COLOR BASIC 0.1.1A\r\n");
    printf("COPR. 2021 BY TREY TOMES\r\n");
    printf("\r\n");
}

inline bool starts_with(const char* haystack, const char* needle) {
    return strstr(haystack, needle) == haystack;
}

bool isinteger(const char* text) {
    for (; *text != 0; text++) {
        if (!isdigit(*text)) {
            return false;
        }
    }
    return true;
}

/**
 * An identifier starts with a letter, and is followed by letters or numbers.
 */
bool isident(const char* text) {
    if (!isalpha(*text)) {
        return false;
    }
    for (; *text != 0; text++) {
        if (!isalpha(text) && !isdigit(text)) {
            return false;
        }
    }
    return true;
}

char* trim_left(char* text) {
    while (isspace(*text)) {
        text++;
    }
    return text;
}

char* trim_right(char* text) {
    int n = strlen(text) - 1;
    while (isspace(text[n])) {
        text[n] = 0;
    }
    return text;
}

void parse_cls(char* line) {
    uint8_t color = terminal_get_cursor_color();
    int bgColor = color >> 4;
    int fgColor = color & 0x0F;

    char* token;

    // Read the command text.
    token = strtok(line, " \t"); // There could be whitespace after the "cls".
    char* cmd = token;
    if (cmd == NULL) {
        printf("?FC ERROR\r\n");
        return;
    }
    
    // Read the background color.
    token = strtok(NULL, ","); // There may or may not be a comma.
    if (token != NULL) {
        token = trim_left(token);
        token = trim_right(token);
        if (!isinteger(token)) {
            printf("?SN ERROR\r\n");
            return;
        }

        bgColor = atoi(token);
        if ((bgColor < 0) || (bgColor > 15)) {
            printf("?FC ERROR\r\n");
            return;
        }

        // Read the foreground color.
        token = strtok(NULL, "");
        if (token != NULL) { // The foreground parameter is optional.
            token = trim_left(token);
            token = trim_right(token);
            if (!isinteger(token)) {
                printf("?SN ERROR\r\n");
                return;
            }

            fgColor = atoi(token);
            if ((fgColor < 0) || (fgColor > 15)) {
                printf("?FC ERROR\r\n");
                return;
            }
        }
    }

    terminal_clear(vga_entry_color(fgColor, bgColor));
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
            } else {
                //char text[32] = {0};
                //itoa(len, text, 32);
                printf("You said: %s (Length: %d)\r\n", line, len);
            }
            printf("OK\r\n");
        }
        
        /*
        if (ch == '\n') {
            printf("\r\n");
        } else if (ch == '\b') {
            // Backspace is handled in tty.
            printf("%c", ch);
        } else {
            printf("%c", ch);
        }
        */
        /*
        char text[32] = {0};
        int n = ch;
        itoa(n, text, 32);
        printf("%s %c\r\n", text, ch);
        */
    }

	/*
	terminal_writestring("It lives!\r\n");
    terminal_writestring("But does it respect line break?\n\n");
    terminal_writestring("\r...does it?\n");
    terminal_writestring("And where are we now?\r\n");

    uint8_t fgColor = VGA_COLOR_LIGHT_GREY;
    for (size_t row = 0; row < VGA_HEIGHT; row++) {
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            uint16_t value = terminal_getentryat(column, row);
            uint8_t ch = value & 0xFF; // Lop off the color bits.
            uint8_t bgColor = (row ^ column) & 0x04;
            uint8_t color = vga_entry_color((vga_color)fgColor, (vga_color)bgColor);
            terminal_putentryat(ch, color, column, row);
        }
    }
	*/

    // Scroll Test
    /*
    uint8_t ch = 'a';
    while (true) {
        terminal_putchar(ch);
        ch++;
        if (ch > 'z') {
            ch = 'a';
        }
        line_feed();
    }
    */
}
