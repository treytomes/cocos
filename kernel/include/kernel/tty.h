#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/vga.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

void cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
void cursor_disable();
void cursor_set_position(int x, int y);
uint16_t cursor_get_position(void);

void terminal_clear(uint8_t color);
void terminal_putchar(char ch);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

size_t terminal_get_cursor_row();
size_t terminal_get_cursor_column();
uint8_t terminal_get_cursor_color();
void terminal_putentryat(unsigned char ch, uint8_t color, size_t x, size_t y);

#endif
