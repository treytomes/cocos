#include "system.h"
//#include "screen.h"
//#include "font.h"

static uint32_t rseed = 1;

void seed(uint32_t s) {
    rseed = s;
}

uint32_t rand() {
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123;

    x *= 23786259 - rseed;

    uint32_t t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

void panic(const char *err) {
    /*
    screen_clear(COLOR(7, 0, 0));

    if (err != NULL) {
        font_str(err, (SCREEN_WIDTH - font_width(err)) / 2, SCREEN_HEIGHT / 2 - 4, COLOR(7, 7, 3));
    }

    screen_swap();
    for (;;) {}
    */
}
