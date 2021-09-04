#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include <kernel/util.h>
#include <kernel/vga.h>

static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static bool terminal_dump_raw = false;

// Cursor

void cursor_enable(uint8_t cursor_start, uint8_t cursor_end) {
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, (inportb(0x3D5) & 0xC0) | cursor_start);
 
	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3D5) & 0xE0) | cursor_end);
}

void cursor_disable() {
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, 0x20);
}

void cursor_set_position(int x, int y) {
	uint16_t pos = y * vga_width + x;
 
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (uint8_t) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t cursor_get_position(void) {
    uint16_t pos = 0;
    outportb(0x3D4, 0x0F);
    pos |= inportb(0x3D5);
    outportb(0x3D4, 0x0E);
    pos |= ((uint16_t)inportb(0x3D5)) << 8;
    return pos;
}

// Terminal

void terminal_clear(uint8_t color) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = color; //vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

	uint16_t fillValue = vga_entry(' ', terminal_color);
	for (size_t y = 0; y < vga_height; y++) {
		for (size_t x = 0; x < vga_width; x++) {
			const size_t index = y * vga_width + x;
			terminal_buffer[index] = fillValue;
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

uint16_t terminal_getentryat(size_t x, size_t y) {
    const size_t index = y * vga_width + x;
    return terminal_buffer[index];
}

size_t terminal_get_cursor_row() {
	return terminal_row;
}

size_t terminal_get_cursor_column() {
	return terminal_column;
}

uint8_t terminal_get_cursor_color() {
	return terminal_color;
}

void terminal_putentryat(unsigned char ch, uint8_t color, size_t x, size_t y) {
	const size_t index = y * vga_width + x;
	terminal_buffer[index] = vga_entry(ch, color);
}

/*
void terminal_scroll(int line) {
	for (uint8_t* loop = (uint8_t*)VGA_MEMORY + line * (VGA_WIDTH * 2); loop < (uint8_t*)VGA_MEMORY + VGA_WIDTH * 2; loop++) {
		uint8_t value = *loop;
		*(loop - (VGA_WIDTH * 2)) = value;
	}
}
 
void terminal_delete_last_line() {
	uint8_t* ptr = (uint8_t*)VGA_MEMORY + (VGA_WIDTH * 2) * (vga_height - 1);
 
	for (size_t x = 0; x < VGA_WIDTH * 2; x++, ptr++) {
		*ptr = 0;
	}
}
*/

void scroll_up() {
    memcpy(terminal_buffer + 0, terminal_buffer + vga_width, (vga_width * vga_height - vga_width) * 2);
    uint16_t space_entry = vga_entry(' ', terminal_color);
    memset(terminal_buffer + vga_width * vga_height - vga_width, space_entry, vga_width * 2);
}

void carriage_return() {
	terminal_column = 0;
}

void line_feed() {
	terminal_row++;
	if (terminal_row == vga_height) {
		/*
		for (size_t line = 1; line <= VGA_HEIGHT - 1; line++) {
			terminal_scroll(line);
		}
		terminal_delete_last_line();
		*/
		scroll_up();
		terminal_row = vga_height - 1;
	}
}

void terminal_putchar(char c) {
	unsigned char uc = c;

	if (terminal_dump_raw) {
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
		terminal_column++;
		if (terminal_column == vga_width) {
			carriage_return();
			line_feed();
		}
		return;
	}

	if (uc == '\r') {
		carriage_return();
	} else if (uc == '\n') {
		line_feed();
	} else if (uc == '\b') {
		if (terminal_column > 0) {
			terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			terminal_column--;
		}
	} else {
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
		terminal_column++;
		if (terminal_column == vga_width) {
			carriage_return();
			line_feed();
		}
	}

	cursor_set_position(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_set_raw(bool value) {
	terminal_dump_raw = value;
}