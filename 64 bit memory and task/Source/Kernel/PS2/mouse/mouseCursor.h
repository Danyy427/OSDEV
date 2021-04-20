#ifndef _MOUSE_CURSOR_H_
#define _MOUSE_CURSOR_H_

#include "stdint.h"

uint8_t MousePointer[];

void DrawMouseCursor(uint32_t x, uint32_t y, uint32_t colour);

void ClearMouseCursor(uint32_t x, uint32_t y);
#endif