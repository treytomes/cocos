#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// This isn't working to print the current year.

char *itoa(unsigned int x, char *s, size_t sz) {
    // TODO: this is bad code we need some error handling here
    if (sz < 20) {
        extern void panic(const char *);
        panic("ITOA BUFFER TOO SMALL");
    }

    unsigned int tmp;
    int i, j;

    tmp = x;
    i = 0;

    do {
        tmp = x % 10;
        s[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (x /= 10);
    s[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return s;
}
/*
char *itoa(unsigned int x, char *s, size_t sz) {
    // TODO: this is bad code we need some error handling here
    if (sz < 20) {
        extern void panic(const char *);
        panic("ITOA BUFFER TOO SMALL");
    }

    const int BASE = 10;
    int numDigits;

    while (true) {
        uint8_t digit = x % 10; //BASE;

        //putchar(digit + '0');

        s[numDigits] = (digit < 10) ? (digit + '0') : (digit + 'A' - 10);
        numDigits++;
        x = (int)(x / 10);
        if (x == 0) {
            break;
        }
    }
    s[numDigits] = 0;

    // Reverse the digits.
    for (int j = 0; j < numDigits; j++) {
        uint8_t tmp = s[j];
        s[j] = s[numDigits - j - 1];
        s[numDigits - j - 1] = tmp;
    }

    return s;
}
*/