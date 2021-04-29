#include "shell.h"
void initShell()
{
	printk("|------------------ WELCOME TO TaraOS SHELL ---------------------------|\n");
	printk("|                                                                      |\n");
	printk("|             Version: 1.0.0     Shell Version: 0.0.1                  |\n");
	printk("|                                                                      |\n");
	printk("|----------------------------------------------------------------------|\n");
	printk("\n");
	printk("@SystemHelper> Type \"help\" to get help.\n");
	getUserInput( );
	
}


int check_command(char* cmd, const char* text)
{
    return strcmp(cmd, (char*)text) == 0;
}
void getUserInput()
{
	unsigned char ch;
	unsigned char *str = kalloc(&currentHeap, 128);
	unsigned int iterator;
	unsigned char otherString[4];
	while(1)
	{
		printk("@Root> ");
		ch = getch();
		iterator = 0;
		while( ch != 13 )
		{
			
			if(ch == 8)
			{
				backspace();
				*(str+iterator) = (char)0; 
				iterator--;
				ch = 0;
				continue;
			}
			
			if(ch != 0)
			{
				*(str+iterator) = ch;
				printk("%c", ch);
				ch = getch();
			
				iterator++;
			}
			else
			{
				
				ch = getch();
			}
			
		}
		
		
		printk("\n");
		printk("%s\n", str);
		//drawStringToCursor(str, 0xffffff, 0x000000);
		if(check_command(str, "help"))
		{
			printk("TaraOS shell help. v0.0.01\n");
			printk("Tara OS version 1.0.0\n");
			printk("Commands: \n");
			printk("help: displays this menu\n");
			printk("info: gives system information\n");
			
		}
		else if(check_command(str, "info"))
		{
			printk("TaraOS version 1.0.0\n");
			printk("Operating System Size %d bytes\n", KERNEL_SIZE*512);
			printk("Current Description: \"64-bit-Shell\" \n");
		
		}
		else
		{
			printk("Unknown command, type \"help\" to learn more\n");
			
		}
		memset(str, 0, 128);
	}


}
