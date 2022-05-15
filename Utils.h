#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *my_realloc(void *__ptr, size_t __size);

char *my_strtok_r(char *s, const char *delim, char **save_ptr);

#endif
