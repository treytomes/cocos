#include <stdio.h>

size_t getline(char** lineptr, size_t* n) {
    size_t len = 0;
    while (len < *n) {
        char ch = getchar();
        if (ch == '\n') {
            break;
        } else if (ch == '\b') {
            if (len > 0) {
                len--;
            }
        } else {
            (*lineptr)[len] = ch;
            len++;
        }
    }
    (*lineptr)[len] = 0;
    return len;
}
