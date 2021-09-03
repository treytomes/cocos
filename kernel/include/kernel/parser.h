#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdbool.h>
#include <string.h>

inline bool starts_with(const char* haystack, const char* needle) {
    return strstr(haystack, needle) == haystack;
}

bool isinteger(const char* text);

/**
 * An identifier starts with a letter, and is followed by letters or numbers.
 */
bool isident(const char* text);

char* trim_left(char* text);

char* trim_right(char* text);

inline char* trim(char* text) {
    return trim_left(trim_right(text));
}

/**
 * Move textPtr to the first non-whitespace character.
 */
void skip_whitespace(char** textPtr);

/**
 * If the next character is ch, then increment textPtr,
 * Otherwise return false.
 */
bool match_char(char** textPtr, char ch);

/**
 * If the next section matched ident, then increment textPtr,
 * Otherwise return false.
 */
bool match_ident(char** textPtr, const char* ident);

/**
 * Read an identifier off of the input text.
 * textPtr will be incremented, then a new string will be allocated
 * for the identifier (letter, followed by number, letter or underscore).
 * 
 * This will return false if textPtr doesn't start with an identifier.
 * 
 * Don't forget to set it free when you're done!
 */
bool read_ident(char** textPtr, char** result);

/**
 * Read an integer off of the input text.
 * textPtr will be incremented, then the result will be converted to an integer.
 * 
 * This will return NULL if textPtr doesn't start with an identifier.
 * 
 * Don't forget to set it free when you're done!
 */
bool read_integer(char** textPtr, int* result);

inline bool is_at_eol(char** textPtr) {
    return **textPtr == 0;
}

#endif
