#ifndef _PMM_H_
#define _PMM_H_
#include "../memory.h"
#include "../../bitmap/bitmap.h"
#include "../../printf/printf.h"

bitmap_t bitmap;

void initPmm();

void* RequestPage();

void ReservePages(void* address, unsigned int pageCount);

void UnreservePages(void* address, unsigned int pageCount);

void FreePages(void* address, unsigned int pageCount);

void LockPages(unsigned long long address, unsigned int pageCount);

uint64_t freeMemory;
uint64_t usedMemory;
uint64_t reservedMemory;

#endif