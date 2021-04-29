


#include "printf.h"
#include "stdint.h"
char buffer[100];
void printk(const char* format, ...) 
{
	char * str = format;
	
	char *ite;
	
	va_list listPointer;

    va_start( listPointer, format );
	
	for (ite = format; *ite != '\0'; ite++) 
	{
		if (*ite == '%') 
		{
			ite++;
			
			switch(*ite)
			{
				case 'c':
					drawCharToCursor(va_arg(listPointer, char*), 0xffffff, 0x000000);
					incrementCursor();
					break;
				case 'd':
					drawStringToCursor(itoa(va_arg(listPointer, uint64_t), buffer, 10), 0xffffff, 0x000000);
					break;
				case 's':
					drawStringToCursor(va_arg(listPointer, char *), 0xffffff, 0x000000);
					break;
				case 'x':
					drawStringToCursor(itoa(va_arg(listPointer, uint64_t), buffer, 16), 0xffffff, 0x000000);
					break;
				
				case '%':
					drawCharToCursor('%', 0xffffff, 0x000000);
					incrementCursor();
					break;
			}
		}
		else
		{
			drawCharToCursor(*ite, 0xffffff, 0x000000);
			if(*ite != '\n') 
				incrementCursor();
		}
	}
	
	va_end(listPointer);
	
	
}


void panic(char* str)
{
	printk("PANIC: %s", str);
}


















