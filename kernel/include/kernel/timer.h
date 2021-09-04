#ifndef TIMER_H
#define TIMER_H

#include "util.h"

uint64_t timer_get();
void timer_init();

/**
 * Wait for a number of ticks
 */
void timer_sleep(uint64_t ticks);

#endif
