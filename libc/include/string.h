#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);


/**
 * Searches for the first occurrence of the character c (an unsigned char)
 * in the string pointed to by the argument str.
 */
char* strchr(const char* str, int ch);

int strcmp(const char* str1, const char* str2);

/**
 * Scans str1 for the first occurrence of any of the characters that are part of str2,
 * returning the number of characters of str1 read before this first occurrence.
 * 
 * The search includes the terminating null-characters.
 * Therefore, the function will return the length of str1 if
 * none of the characters of str2 are found in str1.
 */
size_t strcspn(const char* str1, const char* str2);

size_t strlcat(char *dst, const char *src, size_t size);
size_t strlcpy(char *dst, const char *src, size_t n);
size_t strlen(const char*);

char* strlwr(char* s);

/**
 * Returns the length of the initial portion of str1 which consists only of characters that are part of str2.
 */
size_t strspn(const char* str1, const char* str2);

/**
 * Finds the first occurrence of the substring needle in the string haystack.
 * The terminating '\0' characters are not compared.
 */
char *strstr(const char *haystack, const char *needle);

/* Parse S into tokens separated by characters in DELIM.
   If S is NULL, the last string strtok() was called with is
   used.  For example:
	char s[] = "-abc-=-def";
	x = strtok(s, "-");		// x = "abc"
	x = strtok(NULL, "-=");		// x = "def"
	x = strtok(NULL, "=");		// x = NULL
		// s = "abc\0=-def\0"
*/
char* strtok(char* s, const char* delim);

char* strupr(char* s);

#ifdef __cplusplus
}
#endif

#endif
