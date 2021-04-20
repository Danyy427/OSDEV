#include "bitmap.h"

#define INDEX_FROM_BIT(a) (a / (8 * 8))
#define OFFSET_FROM_BIT(a) (a % (8 * 8))


int getBitmap(bitmap_t bitmap, unsigned long index)
{
    unsigned long idx = INDEX_FROM_BIT(index);
    unsigned long off = OFFSET_FROM_BIT(index);
    return (bitmap.buffer[idx] & (0x1 << off));
}

void setBitmap(bitmap_t bitmap, unsigned long index, int value)
{
	
    unsigned long idx = INDEX_FROM_BIT(index);
    unsigned long off = OFFSET_FROM_BIT(index);
	
    if(value)
    {
        bitmap.buffer[idx] |= (0x1 << off);
		
    }
    else
    {
        bitmap.buffer[idx] &= ~(0x1 << off);
		
    }
}

unsigned long bitmapFirstZeroBit(bitmap_t bitmap)
{
    unsigned int i, j;
    for (i = 0; i < bitmap.size; i++)
    {
        if (bitmap.buffer[i] != 0xFFFFFFFFFFFFFFFF)
        {
            for (j = 0; j < 64; j++)
            {
                unsigned long toTest = 0x1 << j;
                if (!(bitmap.buffer[i] & toTest))
                {
                    return i * 8 * 8 + j;
                }
            }
        }
    }

    return 0xFFFFFFFFFFFFFFFFF;
}








