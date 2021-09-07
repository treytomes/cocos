#include <ctype.h>
#include <string.h>

char* strupr(char* s) {
    for (int n = 0; s[n] != 0; n++) {
        s[n] = toupper(s[n]);
    }
}