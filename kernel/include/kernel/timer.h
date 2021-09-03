#ifndef TIMER_H
#define TIMER_H

#include "util.h"

// number chosen to be integer divisor of PIC frequency
#define TIMER_TPS 363

uint64_t timer_get();
void timer_init();

/**
 * Wait for a number of ticks
 */
void timer_sleep(uint64_t ticks);

#endif
