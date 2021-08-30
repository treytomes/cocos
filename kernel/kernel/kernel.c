#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>

#include "keyboard.h"
#include "timer.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "fpu.h"

__attribute__ ((constructor)) void kernel_premain(void) {
    terminal_initialize();
	printf("The terminal is initialized.\r\n");

    // Make a blocky cursor.
    cursor_enable(0, 15);

    idt_init();
    printf("IDF is initialized.\r\n");

    isr_init();
    printf("ISR is initialized.\r\n");

    fpu_init();
    printf("FPU is initialized.\r\n");

    irq_init();
    printf("IRQ is initialized.\r\n");

    timer_init();
    printf("Timer is initialized.\r\n");

    keyboard_init();
    printf("Keyboard is initialized.\r\n");
}

void kernel_main(void) {
	printf("Hello, kernel World!\r\n");

    while (true) {
        uint8_t ch = keyboard_getchar();
        if (ch != 0) {
            printf("%c", ch);
            /*
            char text[32] = {0};
            int n = ch;
            itoa(n, text, 32);
            printf("%s %c\r\n", text, ch);
            */
        }
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
