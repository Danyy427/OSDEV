#include "./pmm.h"
#include "../../kmain/constants.h"
#include "stdint.h"

void initPmm()
{

	bitmap.buffer = KERNEL_END + 0x1000;
	bitmap.size = 1024*1024/(sizeof(uint64_t));
	
	memset(bitmap.buffer, 0, 1024*1024/(sizeof(uint64_t)))	;
	
	LockPages(0, 0x100);
	
	
	//LockPages((void*)KERNEL_START, (KERNEL_END - KERNEL_START + 0x1000) / 0x1000);
	
	

}



void* RequestPage()
{
    unsigned int index = bitmapFirstZeroBit(bitmap);
	
    if(index == 0xFFFFFFFFFFFFFFFF)
	{
		printk("KERNEL ERROR: no more free pages are available!\n");
        while(1);
    }

    LockPages((void*)(index * 0x1000), 1);
    return (void*)((index * 0x1000));
}

void ReservePages(void* address, unsigned int pageCount)
{
    for(unsigned int i = 0; i < pageCount; i++)
    {
        unsigned int addr = (unsigned int)address + i * 0x1000;

        setBitmap(bitmap, addr / 0x1000, 1);
    }
}

void UnreservePages(void* address, unsigned int pageCount)
{
    for(unsigned int i = 0; i < pageCount; i++)
    {
        unsigned int addr = (unsigned int)address + i * 0x1000;

        setBitmap(bitmap, addr / 0x1000, 0);
    }
}

void FreePages(void* address, unsigned int pageCount)
{
    for(unsigned int i = 0; i < pageCount; i++)
    {
        unsigned int addr = (unsigned int)address + i * 0x1000;

        setBitmap(bitmap, addr / 0x1000, 0);
    }
}

void LockPages(unsigned int address, unsigned int pageCount)
{
	
	
    for(unsigned int i = 0; i < pageCount; i++)
    {
        unsigned int addr = (unsigned int)address + i * 0x1000;

        setBitmap(bitmap, addr / 0x1000, 1);
		
    }
}
