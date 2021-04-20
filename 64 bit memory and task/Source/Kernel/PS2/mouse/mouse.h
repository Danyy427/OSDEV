#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "./mouseCursor.h"

unsigned char readMouse();

unsigned char commandMouse(unsigned char command);

unsigned char initMouse();

unsigned long MouseX;
unsigned long MouseY;
#endif