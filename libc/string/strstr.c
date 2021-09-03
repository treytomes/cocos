#include <string.h>
#include <stdio.h>

char *strstr(const char *haystack, const char *needle) {
    if ((haystack == NULL) || (needle == NULL)) {
        return NULL;
    }

    int needle_len = strlen(needle);
    if (needle_len == 0) {
        return haystack;
    }

    while (*haystack != 0) {
        // Skip ahead to the first matching character in needle.
        haystack = strchr(haystack, needle[0]);
        if (haystack == NULL) {
            return NULL; // The needle start couldn't be found.
        }

        int haystack_len = strlen(haystack);

        // Do we still have enough haystack to hide the needle?
        if (haystack_len < needle_len) {
            return NULL;
        }

        if (memcmp(haystack, needle, needle_len) == 0) {
            return haystack;
        } else {
            // Move ahead a character for the next search.
            haystack++;
        }
    }

    // TODO: Is this right?
    return NULL;
}
