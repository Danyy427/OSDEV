#ifndef _BITMAP_H_p
#define _BITMAP_H_

#include "stdint.h"

typedef struct
{
	uint64_t size;
    uint64_t* buffer;
	
} bitmap_t;

int getBitmap(bitmap_t bitmap, unsigned long index);
void setBitmap(bitmap_t bitmap, unsigned long index, int value);
unsigned long bitmapFirstZeroBit(bitmap_t bitmap);



#endif




