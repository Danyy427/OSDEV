#include "../../io/io.h"
#include "../ps2.h"


unsigned char readMouse()
{
	
	
	return readPS2DataPort();
}

unsigned char commandMouse(unsigned char command)
{
	writeToSecondPS2Port(command);
	controllerWait(0);
	return inb(0x60); 
}

unsigned char initMouse()
{
	
	char res = commandMouse(0xF6);
	if(res == 0xFE) res = commandMouse(0xF6);
	if(res == 0xFC) return 1;
	int try = 3;
	res = commandMouse(0xF4);
	if(res == 0xFE)
	{
		while(res == 0xFE && try != 0)
		{
			res = commandMouse(0xF4);
			try--;
		}
	}
	if(res == 0xFC) return 2;
	return 0;

}