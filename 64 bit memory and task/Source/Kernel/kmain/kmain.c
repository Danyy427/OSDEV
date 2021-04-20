

#include "kmain.h"

int _kmain(struct vbe_mode_info_structure *vbemodeinfo)
{
	

	vbemode = *vbemodeinfo;
	
	
	
	initPmm();
	
	
	initPaging();
	
	initDefaultFont();
	initCursor();
	drawStringToCursor("Successfully initialized Cursor and Font\n",0xffffff, 0x000000);
	
	init_idt();	
	drawStringToCursor("Successfully initialized Interrupts\n",0xffffff, 0x000000);
	
	
	
	
	printk("Hello %d", 15) ;
	
	
	while(1);
	
	return 0;
}


