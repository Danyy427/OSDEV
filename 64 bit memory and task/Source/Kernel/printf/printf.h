#ifndef _PRINTF_H_
#define _PRINTF_H_

#include "../video/video.h"
#include "../args/args.h"

void printk(const char* format, ...) ;
void panic(char* str);

#endif
