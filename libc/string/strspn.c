#include <stdbool.h>
#include <string.h>

/**
 * Returns the length of the initial portion of str1 which consists only of characters that are part of str2.
 */
size_t strspn(const char* str1, const char* str2) {
    size_t len = 0;
    
    for (size_t n1 = 0; str1[n1] != 0; n1++) {
        char ch1 = str1[n1];

        bool match = false;
        for (size_t n2 = 0; str2[n2] != 0; n2++) {
            char ch2 = str2[n2];
            if (ch2 == ch1) {
                match = true;
                break;
            }
        }

        if (!match) {
            break;
        }
        len++;
    }

    return len;
}
