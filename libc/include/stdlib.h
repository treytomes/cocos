#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

int atoi(const char* str, int base);
char *itoa(int x, char *s, int base);

__attribute__((__noreturn__))
void abort(void);

#ifdef __cplusplus
}
#endif

#endif
