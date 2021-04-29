#ifndef _STRING_H_
#define _STRING_H_
#include "stdint.h"

int strlen(const char *str);
char* itoa(uint64_t value, char* buffer, int base);

int strcmp(const char *X, const char *Y);
#endif