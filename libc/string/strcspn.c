#include <stdbool.h>
#include <string.h>

size_t strcspn(const char *str1, const char *str2) {
    size_t len = 0;
    
    for (size_t n1 = 0; str1[n1] != 0; n1++) {
        char ch1 = str1[n1];

        bool match = true;
        for (size_t n2 = 0; str2[n2] != 0; n2++) {
            char ch2 = str2[n2];
            if (ch2 == ch1) {
                match = false;
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
