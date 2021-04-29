#ifndef _HEAP_H_
#define _HEAP_H_

#include "stdint.h"
#include "../memory.h"
typedef struct heap_page{
	uint32_t length;
	struct heap_page *next;
} __attribute__((packed)) heap_block_t;

typedef struct {
	
	unsigned long long address; // page aligned
	unsigned long long size; // in pages;
	unsigned long long used;
	unsigned long long free;
}__attribute__((packed)) heap_t;

heap_t initialiseHeap(uint64_t address);

void* kalloc(heap_t *heap, unsigned int size);

void kfree(heap_t *heap, void * address);

void printHeap(heap_t* heap);

heap_t currentHeap;

#endif