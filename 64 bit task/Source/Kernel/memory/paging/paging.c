


#include "paging.h"
#include "../physical/pmm.h"
#include "../memory.h"
#include "../../printf/printf.h"






void initPaging()
{
	pml4_t *kernelDirectory = RequestPage();
	currentPML4 = kernelDirectory;
	//printk("kDirectory: %x", 0x1222);
	
	unsigned int i;
	for(i = 0; i < 512; i++)
	{
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		kernelDirectory->entries[i].present = 0;
		kernelDirectory->entries[i].writeable = 1;
		kernelDirectory->entries[i].user_access = 0;
		kernelDirectory->entries[i].write_through = 0;
		kernelDirectory->entries[i].cache_disabled = 0;
		kernelDirectory->entries[i].accessed = 0;
		kernelDirectory->entries[i].zero = 0;
		kernelDirectory->entries[i].size = 0;
		kernelDirectory->entries[i].global = 0;
		kernelDirectory->entries[i].ignored_2 = 0;
		kernelDirectory->entries[i].address = 0x0000000;
		kernelDirectory->entries[i].reserved_1 = 0x000;
		kernelDirectory->entries[i].ignored_1 = 0x000;
		kernelDirectory->entries[i].execution_disabled = 0;
	}		
	
	unsigned long long from = 0x0000000000000000, size = 0x0000000006000000,  zz = 0;
	
	for(; size>0; from += 4096, size -= 4096, zz++){
       //first_page_table[zz] = from | 1;     // mark page present.
	   map_page(kernelDirectory, from, from, 0x003);
	   
	 }
	   
	   
	from = 0x00000000fd000000, size = 0x00000000007E9000,  zz = 0;
	for(; size>0; from += 4096, size -= 4096, zz++){
       //first_page_table[zz] = from | 1;     // mark page present.
	   map_page(kernelDirectory, from, from, 0x003);
	   
	   
    }
	
	
	//while(1);
	vbemode.framebuffer = 0xfd000000;
	
	setAsCurrentPML4(kernelDirectory);
	
	//printk("Hello?");
	//enablePaging(kernelDirectory);
	//while(1);
	from = 0x0000000000000000, size = 0x0000000010000000 ,  zz = 0;
	
	for(; size>0; from += 4096, size -= 4096, zz++){
       //first_page_table[zz] = from | 1;     // mark page present.
	   map_page(kernelDirectory, from, from, 0x003);
	   
	 }
	
	//setAsCurrentPML4(kernelDirectory);
}

	
static inline void __native_flush_tlb_single(unsigned int addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void map_page(pml4_t *pml4, void * physaddr, unsigned long long virtualaddr, unsigned int flags)
{
    // Make sure that both addresses are page-aligned.

    unsigned int pdindex;
    unsigned int ptindex;
	unsigned int pdptindex;
	unsigned int pml4index;
	
	virtualaddr >>= 12;
    ptindex = virtualaddr & 0x1ff;
    virtualaddr >>= 9;
    pdindex = virtualaddr & 0x1ff;
    virtualaddr >>= 9;
    pdptindex = virtualaddr & 0x1ff;
    virtualaddr >>= 9;
    pml4index = virtualaddr & 0x1ff;
	
	page_directory_pointer_table_t *pdpt;
	if(pml4->entries[pml4index].present == 0)
	{
		pdpt = RequestPage();
		memset(pdpt, 0, 4096);
		pml4->entries[pml4index].present = 1;
		pml4->entries[pml4index].writeable = 1;
		pml4->entries[pml4index].address = ((unsigned long)pdpt >> 12);
	}	
	else
	{
	
		pdpt = (pml4->entries[pml4index].address << 12);
	}
	
	page_directory_t *directory;
	if(pdpt->entries[pdptindex].present == 0)
	{
		directory = RequestPage();
		memset(directory, 0, 4096);
		pdpt->entries[pdptindex].present = 1;
		pdpt->entries[pdptindex].writeable = 1;
		pdpt->entries[pdptindex].address = ((unsigned long)directory >> 12);
	}	
	else
	{
	
		directory = (pdpt->entries[pdptindex].address << 12);
	}
	
	
	page_table_t *pt;
	if(directory->entries[pdindex].present == 0)
	{
		pt = RequestPage();
		memset(pt, 0, 4096);
		directory->entries[pdindex].present = 1;
		directory->entries[pdindex].writeable = 1;
		directory->entries[pdindex].address = ((unsigned long)pt >> 12);
	}	
	else
	{
	
		pt = (directory->entries[pdindex].address << 12);
	}
    
		
		
	pt->entries[ptindex].present = flags & 1;                 
    pt->entries[ptindex].writeable = (flags & 2) >> 1;               
    pt->entries[ptindex].user_access = (flags & 4) >> 2;         
    pt->entries[ptindex].write_through = (flags & 8) >> 3;          
    pt->entries[ptindex].cache_disabled = (flags & 16) >> 4;           // 000
    pt->entries[ptindex].accessed = (flags & 32) >> 5;               // 0000 0000 0000
	pt->entries[ptindex].dirty = (flags & 64) >> 6;                
	pt->entries[ptindex].zero = 0;                
    pt->entries[ptindex].global = (flags & 256) >> 8;           
    pt->entries[ptindex].ignored_2 = 0b000; 
	pt->entries[ptindex].address = ((unsigned long)physaddr >> 12);
	
	
	
	__native_flush_tlb_single((unsigned long long)pt->entries[ptindex].address << 12);
	 
}

void * get_physaddr(pml4_t *pml4, uint64_t virtualaddr)
{
	
    unsigned int pdindex;
    unsigned int ptindex;
	unsigned int pdptindex;
	unsigned int pml4index;
	
	virtualaddr >>= 12;
    ptindex = virtualaddr & 0x1ff;
    virtualaddr >>= 9;
    pdindex = virtualaddr & 0x1ff;
    virtualaddr >>= 9;
    pdptindex = virtualaddr & 0x1ff;
    virtualaddr >>= 9;
    pml4index = virtualaddr & 0x1ff;
	
	page_directory_pointer_table_t *pdpt = (pml4->entries[pml4index].address << 12);
	page_directory_t *directory = (pdpt->entries[pdptindex].address << 12);
	page_table_t *pt = (directory->entries[pdindex].address << 12);
	
	return (pt->entries[ptindex].address);
	
}



void setAsCurrentPML4(pml4_t *pml4)
{
	//while(1);
    asm volatile("mov %0, %%cr3" : : "r"(pml4));

	currentPML4 = pml4;
}

pml4_t *getCurrentPML4()
{
	return currentPML4;
}
