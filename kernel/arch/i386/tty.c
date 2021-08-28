#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

	uint16_t fillValue = vga_entry(' ', terminal_color);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = fillValue;
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

uint16_t terminal_getentryat(size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    return terminal_buffer[index];
}

void terminal_putentryat(unsigned char ch, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(ch, color);
}

void terminal_scroll(int line) {

	for (uint8_t* loop = (uint8_t*)VGA_MEMORY + line * (VGA_WIDTH * 2); loop < (uint8_t*)VGA_MEMORY + VGA_WIDTH * 2; loop++) {
		uint8_t value = *loop;
		*(loop - (VGA_WIDTH * 2)) = value;
	}
}
 
void terminal_delete_last_line() {
	uint8_t* ptr = (uint8_t*)VGA_MEMORY + (VGA_WIDTH * 2) * (VGA_HEIGHT - 1);
 
	for (size_t x = 0; x < VGA_WIDTH * 2; x++, ptr++) {
		*ptr = 0;
	}
}

void carriage_return() {
	terminal_column = 0;
}

void line_feed() {
	terminal_row++;
	if (terminal_row == VGA_HEIGHT) {
		for (size_t line = 1; line <= VGA_HEIGHT - 1; line++) {
			terminal_scroll(line);
		}
		terminal_delete_last_line();
		terminal_row = VGA_HEIGHT - 1;
	}
}

void terminal_putchar(char c) {
	unsigned char uc = c;

	if (uc == '\r') {
		carriage_return();
	} else if (uc == '\n') {
		line_feed();
	} else {
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
		terminal_column++;
		if (terminal_column == VGA_WIDTH) {
			carriage_return();
			line_feed();
		}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
