#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

static const size_t VGA_WIDTH;
static const size_t VGA_HEIGHT;

void terminal_initialize(void);
void terminal_putchar(char ch);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
