#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>

__attribute__ ((constructor)) void kernel_premain(void) {
	terminal_initialize();
	printf("The terminal is initialized.\r\n");
}

void kernel_main(void) {
	printf("Hello, kernel World!\n");

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
