#include <ctype.h>

int toupper(int c) {
    if ((c >= 'a') && (c <= 'z')) {
        c = c - 'a' + 'A';
    }
    return c;
}