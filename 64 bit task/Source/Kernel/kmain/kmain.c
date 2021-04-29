

#include "kmain.h"

int _kmain(struct vbe_mode_info_structure *vbemodeinfo, memory_map_entry_t* memory_map)
{
	

	vbemode = *vbemodeinfo;
	
	currentMemoryMap = memory_map;
	
	initPmm();
	
	
	initPaging();
	
	initDefaultFont();
	initCursor();
	drawStringToCursor("Successfully initialized Cursor and Font\n",0xffffff, 0x000000);
	
	initGDT();
	init_idt();	
	
	
	
	
	
	
	drawStringToCursor("Successfully initialized Interrupts\n",0xffffff, 0x000000);
	
	
	
	heap_t heap = initialiseHeap(0x300000);
	
	initTasking();
	
	printk("Something about something else");
	
	while(1) {printk("c"); for(int i = 0; i<1000000; i++);
	}
	while(1);
	
	return 0;
} 


