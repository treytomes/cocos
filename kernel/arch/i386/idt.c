#include <kernel/idt.h>
#include <string.h>

#define PACKED __attribute__((packed))

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t __ignored;
    uint8_t type;
    uint16_t offset_high;
} PACKED;

struct IDTPointer {
    uint16_t limit;
    uintptr_t base;
} PACKED;

static struct {
    struct IDTEntry entries[256];
    struct IDTPointer pointer;
} idt;

// in start.S
extern void idt_load();

void idt_set(uint8_t index, void (*base)(struct Registers*), uint16_t selector, uint8_t flags) {
    idt.entries[index] = (struct IDTEntry) {
        .offset_low = ((uintptr_t) base) & 0xFFFF,
        .offset_high = (((uintptr_t) base) >> 16) & 0xFFFF,
        .selector = selector,
        .type = flags | 0x60,
        .__ignored = 0
    };
}

void idt_init() {
    idt.pointer.limit = sizeof(idt.entries) - 1;
    idt.pointer.base = (uintptr_t) &idt.entries[0];
    memset(&idt.entries[0], 0, sizeof(idt.entries));
    idt_load((uintptr_t) &idt.pointer);
}
