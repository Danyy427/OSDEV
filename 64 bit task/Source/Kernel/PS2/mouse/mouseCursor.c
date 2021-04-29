

#include "stdint.h"
#include "mouseCursor.h"
uint8_t MousePointer[] = 
{
    0b10000000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b11110000, 0b00000000, 
    0b11111000, 0b00000000, 
    0b11111110, 0b00000000, 
    0b11111111, 0b00000000, 
    0b11111111, 0b11000000, 
    0b11111111, 0b11100000, 
    0b11111111, 0b11110000, 
    0b11111111, 0b10000000, 
    0b11100011, 0b10000000, 
    0b10000011, 0b11000000, 
    0b00000011, 0b11000000, 
    0b00000001, 0b11100000, 
    0b00000001, 0b11000000, 
    0b00000000, 0b00000000, 
};


int MouseDrawn = 0;
uint32_t MouseCursorBuffer[16 * 16];
uint32_t MouseCursorBufferAfter[16 * 16];
void DrawMouseCursor(uint32_t x, uint32_t y, uint32_t colour)
{
    int xMax = 16;
    int yMax = 16;
    int differenceX = 1920 - x;
    int differenceY = 1080 - y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int _y = 0; _y < yMax; _y++)
    {
        for (int _x = 0; _x < xMax; _x++)
        {
            int bit = _y * 16 + _x;
            int byte = bit / 8;
            if ((MousePointer[byte] & (0b10000000 >> (_x % 8))))
            {
                MouseCursorBuffer[_x + _y * 16] = getpixel(x + _x, y + _y);
                putpixel(x + _x, y + _y, colour);
                MouseCursorBufferAfter[_x + _y * 16] = getpixel(x + _x, y + _y);

            }
        }
    }

    MouseDrawn = 1;
}

void ClearMouseCursor(uint32_t x, uint32_t y)
{
    if (!MouseDrawn) 
    {
        return;
    }

    int xMax = 16;
    int yMax = 16;
    int differenceX = 1920 - x;
    int differenceY = 1080 - y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int _y = 0; _y < yMax; _y++)
    {
        for (int _x = 0; _x < xMax; _x++)
        {
            int bit = _y * 16 + _x;
            int byte = bit / 8;
            if ((MousePointer[byte] & (0b10000000 >> (_x % 8))))
            {
                if (getpixel(x + _x, y + _y) == MouseCursorBufferAfter[_x + _y *16])
                {
                    putpixel(x + _x, y + _y, MouseCursorBuffer[_x + _y * 16]);
                }
            }
        }
    }
}