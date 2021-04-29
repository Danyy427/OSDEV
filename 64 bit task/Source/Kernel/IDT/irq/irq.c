
#include "irq.h"
#include "../../PS2/ps2.h"
#include "../../PS2/keyboard/keyboard.h"
#include "../../PS2/mouse/mouse.h"
#include "../../video/video.h" 
#include "../../string/string.h"
#include "../../task/task.h"

void default_irq_handler(interrupt_frame_t *frame)
{
        /* If IRQ # on slave PIC send EOI to slave */
        if(frame->int_no >= 8);
			outb(0xA0,0x20);
		
        /* Send EOI to master PIC */
        outb(0x20,0x20);
		
}

/* -------------------------- Programable Interval Timer ------------------------- */



void irq0_handler(interrupt_frame_t* frame)
{
	EnvironmentTick++;
	//printRegisters((task_registers_t*)frame);
	
	schedule((task_registers_t*)frame);
	
	default_irq_handler(frame);
}

void Sleep(int ticks)
{
	int now = EnvironmentTick;

	while (EnvironmentTick != now + ticks);

	return;

}
void SetPITSpeed(int hz)
{
	int divisor = 1193180 / hz;       /* Calculate our divisor */
	outb(0x43, 0x36);             /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}
/*  ------------------------------------------------------------------------------ */


/* -------------------------- Keyboard Controller -------------------------------- */
unsigned char currentCharInScancode;
unsigned char currentCharInKeycode;
unsigned char currentCharInASCII;

int isE0 = 0;
int isE1 = 0;
int pause = 0;
int prntscrn = 0;	
int isKeyPressed = 0;
int ignore = 0;
int isShift = 0;
char kbd_buffer[100];
int bufferIterator = 0;
char buffer[8];
int isGetchPress = 0;


void irq1_handler(interrupt_frame_t* frame)
{
	
	currentCharInScancode = readKeyboard();
	
	if(ignore > 0)
	{
		ignore--;
		default_irq_handler(frame);
		return;
	}
	else if(currentCharInScancode == 0xE1 && pause == 0)
	{
		pause = 1;
		ignore = 5;
	}
	else if(currentCharInScancode == 0xE0)
	{
		isE0 = 1;
		default_irq_handler(frame);
		return;
	}
	else if(currentCharInScancode == 0xF0)
	{
		isKeyPressed = 2;
		default_irq_handler(frame);
		return;
	}
	else if(currentCharInScancode == 0x7C && isE0 == 1)
	{	
		prntscrn = 1;
		ignore = 2;
	}
	
	
	currentCharInKeycode = ScancodeToKeyCode(currentCharInScancode, isE0, prntscrn, pause);
	if(isE0 == 1) isE0 = 0;
	if(pause == 1) pause = 0;
	if(prntscrn == 1) prntscrn = 0;
	
	kbd_buffer[bufferIterator] = currentCharInKeycode;
	if(bufferIterator == 99) bufferIterator = 0;
	else bufferIterator++;
	
	if(isKeyPressed == 0)
	{
		isKeyPressed = 1;
		isGetchPress = 1;
	}
	else if(isKeyPressed == 2)
	{
		isKeyPressed = 0;
	}
	
	
	
	//drawStringToCursor(itoa(isKeyPressed, buffer, 16), 0xffffff, 0x000000);
	
	KeyPressed[currentCharInKeycode] = isKeyPressed; 
	
	//drawStringToCursor(itoa(isKeyPressed, buffer, 16), 0xffffff, 0x000000);
	
	
	
	/*
	if((KeyCodeToASCII(currentCharInKeycode, KeyPressed[LSHIFT] | KeyPressed[RSHIFT]) != 0) && isKeyPressed == 1) 
	{
		drawCharToCursor(KeyCodeToASCII(currentCharInKeycode, KeyPressed[LSHIFT] | KeyPressed[RSHIFT]), 0xffffff, 0x000000);
		incrementCursor();
	}*/
	default_irq_handler(frame);
}


unsigned char getkbd()
{
	unsigned char ch;
	try_again:
	while(isGetchPress != 1);
	ch = currentCharInKeycode;
	isGetchPress = 0;
	
	if(ch == 0) goto try_again;
	return ch;

}

unsigned char getch()
{
	unsigned char ch;
	try_again:
		ch = KeyCodeToASCII(getkbd(),  KeyPressed[LSHIFT] | KeyPressed[RSHIFT],  KeyPressed[LALT] | KeyPressed[RALT]);
	if(ch == 0) goto try_again;
	return ch;
	
}


/*  ------------------------------------------------------------------------------ */

 
void irq2_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq3_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq4_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq5_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq6_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq7_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq8_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq9_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq10_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq11_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}

// -------------------------------- Mouse Controller ------------------------

char mouse_byte[3];
int mousePacketReady = 0;
int mouse_cycle = 0;
signed int MousePositionOldX = 0, MousePositionOldY = 0;
int RightButton, MiddleButton, LeftButton;

static void ProcessMousePacket()
{
    if (!mousePacketReady)
    {
        return;
    }

    unsigned char xNeg = mouse_byte[0] & 0b00010000;
    unsigned char yNeg = mouse_byte[0] & 0b00100000;
    unsigned char xOF = mouse_byte[0] & 0b01000000;
    unsigned char yOF = mouse_byte[0] & 0b10000000;

    if (!xNeg)
    {
        MouseX += mouse_byte[1];

        if (xOF)
        {
            MouseX += 255;
        }
    } 
    else
    {
        mouse_byte[1] = 256 - mouse_byte[1];
        MouseX -= mouse_byte[1];

        if (xOF)
        {
            MouseX -= 255;
        }
    }

    if (!yNeg)
    {
        MouseY -= mouse_byte[2];

        if (yOF)
        {
            MouseY -= 255;
        }
    } else
    {
        mouse_byte[2] = 256 - mouse_byte[2];
        MouseY += mouse_byte[2];

        if (yOF)
        {
            MouseY += 255;
        }
    }

    if (MouseX < 0) MouseX = 0;
    if (MouseX > 1920 - 1) MouseX = 1920 - 1;
    
    if (MouseY < 0) MouseY = 0;
    if (MouseY > 1080 - 1) MouseY = 1080 - 1;
    
    //putpixel(MouseX, MouseY, 0xffffff);
	
	ClearMouseCursor(MousePositionOldX, MousePositionOldY);
	DrawMouseCursor(MouseX, MouseY, 0xffffff);

	
    if (mouse_byte[0] & 0b00000001)
    {
        LeftButton = 1;
    }
    else
    {
        LeftButton = 0;
    }

    if (mouse_byte[0] & 0b00000100)
    {
        MiddleButton = 1;
    }
    else
    {
        MiddleButton = 0;
    }

    if (mouse_byte[0] &  0b00000010)
    {
        RightButton = 1;
    }
    else
    {
        RightButton = 0;
    }
    mousePacketReady = 0;
    MousePositionOldX = MouseX;
    MousePositionOldY = MouseY;
}
int skip = 1;
void irq12_handler(interrupt_frame_t* frame)
{
	unsigned char packet = readMouse();
	ProcessMousePacket();
	
	switch(mouse_cycle)
	{
		case 0:
			if ((packet & 0b00001000) == 0) 
            break;
			mouse_byte[0]=packet;
			mouse_cycle++;
			break;
		case 1:
			mouse_byte[1]=packet;
			mouse_cycle++;
			break;
		case 2:
			mouse_byte[2]=packet;
			mousePacketReady = 1;
			mouse_cycle=0;
			break;
	}
	default_irq_handler(frame);
}
// --------------------------------------------------------------------------

void irq13_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq14_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}
void irq15_handler(interrupt_frame_t* frame)
{
	default_irq_handler(frame);
}