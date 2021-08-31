#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stddef.h>

void free(void* ptr);
void *malloc(size_t size);
void* calloc(size_t num, size_t len);
void* realloc(void *ptr, size_t new_size);

#endif