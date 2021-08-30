#include <stdio.h>

size_t gets(char** lineptr, size_t* n) {
    return getline(lineptr, n);
}
