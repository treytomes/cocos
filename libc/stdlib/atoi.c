#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int atoi(const char* str) {
    while (isspace(*str)) {
        str++;
    }

    bool negative = false;
    if (*str == '+') {
        str++;
    } else if (*str == '-') {
        negative = true;
        str++;
    }

    int result = 0;
    
    for (; *str != 0; str++) {
        if (!isdigit(*str)) {
            break;
        }
        result = (result * 10) + (*str - '0');
    }

    return negative ? -result : result;
}