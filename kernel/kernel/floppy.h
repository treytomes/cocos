#ifndef __FLOPPY_H__
#define __FLOPPY_H__
#include <kernel/floppy.h>

bool floppy_reset(int base);
int floppy_read_track(int base, unsigned cyl);
int floppy_write_track(int base, unsigned cyl);

// initializes the floppy
void floppy_init();

#endif