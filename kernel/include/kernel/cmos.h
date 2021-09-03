#ifndef __CMOS_H__
#define __CMOS_H__

#include <stdint.h>
#include <kernel/util.h>

// Are non-maskable interrupts disabled?
const bool NMI_DISABLE = 0;

static inline void cmos_select_register(uint8_t registerNumber) {
    outportb(0x70, (NMI_DISABLE << 7) | registerNumber);
    // TODO: Do I need some type of delay here?
}

/**
 * Selected CMOS register is reset to 0x0D after this call.
 */
static inline uint8_t cmos_read() {
    return inportb(0x71);
}

#endif