#include "./heap.h"


// returns address for heap	
heap_t initialiseHeap(uint64_t address)
{

	heap_t heap;
	heap.address = address; 
	map_page(currentPML4, RequestPage(), address, 0x03);
	heap.size = 1;
	
	heap_block_t * heap_blockk = heap.address;
	
	heap_blockk->length = 0x80000010;
	heap_blockk->next = 0x0;
	heap.free = 4096-(sizeof(heap_block_t))-(heap_blockk->length & 0x7FFFFFFF);
	//heap.lastBlock = heap_blockk;
	currentHeap = heap;
	//printk("the first block is init at %x and lasts until %x", heap_blockk, ((unsigned long)heap_blockk)+12+(heap_blockk->length & 0x7FFFFFFF) );
	return heap;

}
	
void expandHeap(heap_t *heap)
{
	unsigned long long heapStart = heap->address;
	
	unsigned long long extension = RequestPage();
	
	map_page(currentPML4, extension, heapStart+(heap->size*4096), 0x03);
	
	heap->size += 1;
	heap->free += 4096;
	
	return;
}
	
void* kalloc(heap_t *heap, unsigned int size)
{
	
	for(heap_block_t *i = heap->address; i; i = i->next)
	{	
		
		if(((i->length & 0x80000000) >> 31) == 1) // There is a non free space
		{
			//printk("Non free block at %x, length: %x\n", i, i->length & 0x7FFFFFFF);
			if(i->next == 0x0)                    // there is no block after it so lets create one.
			{
				//printk("No more free blocks apparently... will attempt to create one\n");
				unsigned int totalSpace = size+sizeof(heap_block_t);
				if(heap->free > totalSpace)
				{
				
					//printk("%x, %d, %d", (unsigned long long) i, sizeof(heap_block_t), (i->length & 0x7FFFFFFF));
					heap_block_t *newFreeBlock = ((unsigned long long)i)+sizeof(heap_block_t)+(i->length & 0x7FFFFFFF);
					newFreeBlock->length = size & 0x7FFFFFFF;
					newFreeBlock->next = i->next;
					i->next = newFreeBlock;
					//printk("it is ready\n");
					
					heap->free -= totalSpace;
					
					
					continue;
				}
				else
				{
					//printk("whoopsy no more free space in heap, attempting to get more\n");
					
					for(int z = 0; z < ((totalSpace - (totalSpace % 4096))/4096); z++)
						expandHeap(heap);
						
						
					heap_block_t *newFreeBlock = ((unsigned long long)i)+sizeof(heap_block_t)+(i->length & 0x7FFFFFFF);
					newFreeBlock->length = size & 0x7FFFFFFF;
					newFreeBlock->next = i->next;
					i->next = newFreeBlock;
					//printk("it is ready\n");
					
					heap->free -= totalSpace;
					
					
					continue;
				}
				
			}
			
			// if there is more blocks after it why bother, lets go.
			continue;
		}
		else if((i->length & 0x7FFFFFFF) >= size)      // there is a block which is free that is bigger then the size we need
		{
			//printk("Found a free block hopefully %d, %d\n",(i->length & 0x7FFFFFFF), size);
			
			if((i->length & 0x7FFFFFFF) == size) // if it is equal size just give it
			{
				i->length |= 0x80000000; 
				//printk("An equal match\n");
				
				memset(((unsigned long long)i)+sizeof(heap_block_t), 0, i->length & 0x7FFFFFFF);
				return ((unsigned long long)i)+sizeof(heap_block_t);
			}
			else if((i->length & 0x7FFFFFFF) >= size+sizeof(heap_block_t))  // or split it in half if there is room for a header
			{
				heap_block_t *newFreeBlock = (unsigned long long)i+size+sizeof(heap_block_t);
				newFreeBlock->length = (i->length & 0x7FFFFFFF)-size-sizeof(heap_block_t);
				newFreeBlock->next = i->next;
				i->length = size;
				i->length |= 0x80000000; 
				i->next = newFreeBlock;
				
				
				memset(((unsigned long long)i)+sizeof(heap_block_t), 0, i->length & 0x7FFFFFFF);
				return ((unsigned long long)i)+sizeof(heap_block_t);
			}
			else
			{
				//printk("there is a free space that is exactly the space you want, but we cannot split it because there is no room for an header\n");
				continue;
			
			}
		}
		else                                     // there is a free block which is smaller then the size we wanted
		{                                        // if it is the last block, lets create a new block, else, skip it entirely
			if(i->next == 0x0)                   // last block, lets create a new one
			{
				//printk("There is a free block that is smaller than the size we want and it is the last block, lets expand it\n");
				unsigned int totalSpace = size+sizeof(heap_block_t);
				if(heap->free > totalSpace)
				{
				
					//printk("%x, %d, %d", (unsigned long long) i, sizeof(heap_block_t), (i->length & 0x7FFFFFFF));
					heap->free -= i->length + (size-i->length);
					
					i->length = size & 0x7FFFFFFF;
					//printk("it is ready\n");
					
					i->length |= 0x80000000; 
					//printk("An equal match\n");
					memset(((unsigned long long)i)+sizeof(heap_block_t), 0, i->length & 0x7FFFFFFF);
					return ((unsigned long long)i)+sizeof(heap_block_t);
					
					
				}
				else
				{
					//printk("whoopsy no more free space in heap, attempting to get more\n");
					
					for(int z = 0; z < ((totalSpace - (totalSpace % 4096))/4096); z++)
						expandHeap(heap);
						
					heap_block_t *newFreeBlock = ((unsigned long long)i)+sizeof(heap_block_t)+(i->length & 0x7FFFFFFF);
					newFreeBlock->length = size & 0x7FFFFFFF;
					newFreeBlock->next = i->next;
					i->next = newFreeBlock;
					//printk("it is ready\n");
					
					heap->free -= totalSpace;
				}
			}
			else
			{
				continue;                        // nothing we can do
			}
		
		}
	}
	
	return 0;

}

void kfree(heap_t *heap, void * address)
{
	heap_block_t *before;
	unsigned int afterFull, beforeFull;
	for(heap_block_t *i = heap->address; i; i = i->next)
	{	
		
		if(((i->length & 0x80000000) >> 31) == 1 && (unsigned long long)i+12 == (unsigned long long)address) 
		{
			//printk("reached the freeing address... freeing\n");
			
			
			if(((i->next->length & 0x80000000) >> 31) == 0 && i->next != 0x0)
			{
				//printk("one after is free, concentrating into this one\n");
				
				i->length += sizeof(heap_block_t) + (i->next->length& 0x7FFFFFFF); 
				i->next = i->next->next;
				memset((unsigned long long)i->next, 0, (i->next->length& 0x7FFFFFFF)+sizeof(heap_block_t));
				
				
				
			}
			else
			{
			
				//printk("whoopsy after\n");
				afterFull = 1;
			}
			
			if(((before->length & 0x80000000) >> 31) == 0)
			{
				//printk("one before is free, concentrating into before\n");
				
				
				before->length += sizeof(heap_block_t) + (i->length & 0x7FFFFFFF); 
				before->next = i->next;
				memset((unsigned long long)i, 0, (i->length & 0x7FFFFFFF)+sizeof(heap_block_t));
				
				
			}
			else
			{	
			
				//printk("whoopsy before\n");
				beforeFull = 1;
				
			}
			
			if(afterFull == 1 && beforeFull == 1)
			{
				//printk("one before and after are no free, setting this space free\n");
				i->length = i->length & 0x7FFFFFFF;
				memset(address, 0, i->length & 0x7FFFFFFF);
			
			}
			
			
			
			return;
		}
		else 
		{
			//printk("whoopsy the address is not this one\n");
		}
		
		before = i;
	}
	
	

	return;
}

void printHeap(heap_t* heap)
{
	unsigned long s = 0;
	printk("Heap start: %x, HeapSize: %d\n", heap->address, heap->size);
	for(heap_block_t *i = heap->address; i; i = i->next)
	{
		printk("Block: %d, Address: %x, Data: %x, Length: %d, free: %d\n", s, (unsigned long long)i, 
															(unsigned long long)i+sizeof(heap_block_t),
															i->length & 0x7FFFFFFF,
															(i->length & 0x80000000)>>31);
		s++;
	}

}

	
	