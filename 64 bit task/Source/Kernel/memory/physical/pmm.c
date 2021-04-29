#include "./pmm.h"
#include "../../kmain/constants.h"
#include "stdint.h"

void initPmm()
{
	
	uint64_t memorySize = getTotalMemory(currentMemoryMap);
    freeMemory = memorySize;
    usedMemory = 0;
    reservedMemory = 0;
    uint64_t frameCount = memorySize / 0x1000;
    frameCount /= 64;
    frameCount += 1;

	
	
	
    bitmap.buffer  = KERNEL_END +0x1000;
	bitmap.size  = frameCount*sizeof(uint64_t);
    memset(bitmap.buffer, 0, frameCount * sizeof(uint64_t));

   uint64_t *ptr = ((unsigned int)currentMemoryMap)-4;
	uint64_t count = *ptr & 0xFF;	

    for(uint64_t i = 0; i < count; i++)
    {
        if(currentMemoryMap[i].type != 1)
        {
            
            LockPages((void*)currentMemoryMap[i].base, currentMemoryMap[i].length / 0x1000);
        }
    }

    //Lock the first 256 pages
    LockPages(0x0, 0x100);

	
	
	LockPages((void*)KERNEL_START, (KERNEL_END - KERNEL_START + 0x1000) / 0x1000);
	
	
    LockPages(bitmap.buffer, frameCount * sizeof(uint64_t) / 0x1000 + 1);

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
		freeMemory -= 0x1000;
        reservedMemory += 0x1000;
    }
}

void UnreservePages(void* address, unsigned int pageCount)
{
    for(unsigned int i = 0; i < pageCount; i++)
    {
        unsigned int addr = (unsigned int)address + i * 0x1000;

        setBitmap(bitmap, addr / 0x1000, 0);
		freeMemory += 0x1000;
        reservedMemory -= 0x1000;
    }
}

void FreePages(void* address, unsigned int pageCount)
{
    for(unsigned int i = 0; i < pageCount; i++)
    {
        unsigned long long addr = (unsigned long long)address + i * 0x1000;

        setBitmap(bitmap, addr / 0x1000, 0);
		freeMemory += 0x1000;
        usedMemory -= 0x1000;
    }
}

void LockPages(unsigned long long address, unsigned int pageCount)
{
	
	
    for(unsigned int i = 0; i < pageCount; i++)
    {
        unsigned long long addr = (unsigned  long long)address + i * 0x1000;

        setBitmap(bitmap, addr / 0x1000, 1);
		
        freeMemory -= 0x1000;
        usedMemory += 0x1000;
    }
}
