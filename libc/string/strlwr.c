#include <ctype.h>
#include <string.h>

char* strlwr(char* s) {
    for (int n = 0; s[n] != 0; n++) {
        s[n] = tolower(s[n]);
    }
}