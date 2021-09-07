#include <ctype.h>

int tolower(int c) {
    if ((c >= 'A') && (c <= 'Z')) {
        c = c - 'A' + 'a';
    }
    return c;
}