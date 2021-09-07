#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int atoi(const char* str, int base) {
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
        char digit = *str;
        if ((digit >= '0') && (digit <= '9')) {
            digit -= '0';
        } else if (base > 10) {
            if ((digit >= 'a') && (digit < 'a' + base)) {
                digit = digit - 'a' + 10;
            } else if ((digit >= 'A') && (digit < 'A' + base)) {
                digit = digit - 'A' + 10;
            } else {
                break;
            }
        } else {
            break;
        }
        result = (result * base) + digit;
    }

    return negative ? -result : result;
}