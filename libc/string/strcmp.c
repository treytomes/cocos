#include <string.h>

int strcmp(const char* str1, const char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int len = (len1 < len2) ? len1 : len2;

    for (int n = 0; n < len; n++) {
        char ch1 = str1[n];
        char ch2 = str2[n];

        if (ch1 < ch2) {
            return -1;
        } else if (ch1 > ch2) {
            return 1;
        }
    }

    if (len1 < len2) {
        return -1;
    } else if (len1 > len2) {
        return 1;
    } else {
        return 0;
    }
}