#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

int atoi(const char* str);
char *itoa(unsigned int x, char *s, size_t sz);

__attribute__((__noreturn__))
void abort(void);

#ifdef __cplusplus
}
#endif

#endif
