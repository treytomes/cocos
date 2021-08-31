#include <string.h>

#include <stdio.h>

/* Parse S into tokens separated by characters in DELIM.
   If S is NULL, the last string strtok() was called with is
   used.  For example:
	char s[] = "-abc-=-def";
	x = strtok(s, "-");		// x = "abc"
	x = strtok(NULL, "-=");		// x = "def"
	x = strtok(NULL, "=");		// x = NULL
		// s = "abc\0=-def\0"
*/
char* strtok(char* s, const char* delim) {
    static char* save_ptr;

    char *end;

    // If no string is passed in then use the last one.
    if (s == NULL) {
        s = save_ptr;
    }

    // If the string is still null or empty then we're done.
    if ((s == NULL) || (*s == '\0')) {
        save_ptr = s;
        return NULL;
    }

    // Scan leading delimiters.
    s += strspn(s, delim);
    if (*s == '\0')
    {
        save_ptr = s;
        return NULL;
    }

    // Find the end of the token.
    end = s + strcspn(s, delim);
    if (*end == '\0')
    {
        save_ptr = end;
        return s;
    }

    // Terminate the token and make *SAVE_PTR point past it.
    *end = '\0';
    save_ptr = end + 1;
    return s;
}