#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include "isr.h"

void idt_set(uint8_t index, void (*base)(struct Registers*), uint16_t selector, uint8_t flags);
void idt_init();

#endif
