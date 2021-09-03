#include <kernel/parser.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

bool isinteger(const char* text) {
    for (; *text != 0; text++) {
        if (!isdigit(*text)) {
            return false;
        }
    }
    return true;
}

/**
 * An identifier starts with a letter, and is followed by letters or numbers.
 */
bool isident(const char* text) {
    if (!isalpha(*text)) {
        return false;
    }
    for (; *text != 0; text++) {
        if (!isalpha(*text) && !isdigit(*text)) {
            return false;
        }
    }
    return true;
}

char* trim_left(char* text) {
    while (isspace(*text)) {
        text++;
    }
    return text;
}

char* trim_right(char* text) {
    int n = strlen(text) - 1;
    while (isspace(text[n])) {
        text[n] = 0;
    }
    return text;
}

/**
 * Move textPtr to the first non-whitespace character.
 */
void skip_whitespace(char** textPtr) {
    char* text = *textPtr;
    while (isspace(*text)) {
        text++;
    }
    *textPtr = text;
}

/**
 * If the next character is ch, then increment textPtr,
 * Otherwise return false.
 */
bool match_char(char** textPtr, char ch) {
    char* text = *textPtr;
    if (*text == ch) {
        text++;
        *textPtr = text;
        return true;
    }
    return false;
}

/**
 * If the next section matched ident, then increment textPtr,
 * Otherwise return false.
 */
bool match_ident(char** textPtr, const char* ident) {
    // TODO: Should I validate that ident is really an identifier?
    char* start = *textPtr;
    char* end = *textPtr;

    if (!isalpha(*end)) {
        return false;
    }
    end++;

    while ((isalpha(*end) || isdigit(*end) || (*end == '_')) && (*end != 0)) {
        end++;
    }

    int dstLen = end - start;
    int srcLen = strlen(ident);

    if (dstLen != srcLen) {
        return false;
    }
    
    if (memcmp(start, ident, srcLen) == 0) {
        *textPtr = end;
        return true;
    }
    return false;
}

/**
 * Read an identifier off of the input text.
 * textPtr will be incremented, then a new string will be allocated
 * for the identifier (letter, followed by number, letter or underscore).
 * 
 * This will return false if textPtr doesn't start with an identifier.
 * 
 * Don't forget to set it free when you're done!
 */
bool read_ident(char** textPtr, char** result) {
    // TODO: Merge this logic with match_ident, somehow.

    char* start = *textPtr;
    char* end = *textPtr;

    if (!isalpha(*end)) {
        return false;
    }
    end++;

    while ((isalpha(*end) || isdigit(*end) || (*end == '_')) && (*end != 0)) {
        end++;
    }
    int length = end - start;
    *result = malloc(length);
    memcpy(*result, start, length);
    *textPtr = end;
    return true;
}

/**
 * Read an integer off of the input text.
 * textPtr will be incremented, then the result will be converted to an integer.
 * 
 * This will return NULL if textPtr doesn't start with an identifier.
 * 
 * Don't forget to set it free when you're done!
 */
bool read_integer(char** textPtr, int* result) {
    char* start = *textPtr;
    char* end = *textPtr;

    if (!isdigit(*end)) {
        return false;
    }
    end++;

    while (isdigit(*end) && (*end != 0)) {
        end++;
    }
    *result = atoi(start);
    *textPtr = end;
    return true;
}
