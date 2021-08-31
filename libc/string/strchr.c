#include <string.h>

/**
 * Searches for the first occurrence of the character c (an unsigned char)
 * in the string pointed to by the argument str.
 */
char* strchr(const char* str, int ch) {
    for (int n = 0; str[n] != 0; n++) {
        if (str[n] == ch) {
            return (str + n);
        }
    }
    return NULL;
}
