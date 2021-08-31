#include <stdlib.h>
#include <stdbool.h>

int atoi(const char* str) {
    int len = strlen(str);
    int result = 0;
    bool negative = (str[0] == '-');

    for (int n = (negative ? 1 : 0); n < len; n++) {
        if (!isdigit(str[n])) {
            result = 0;
            break;
        }
        result = (result * 10) + (str[n] - '0');
    }

    return negative ? -result : result;
}