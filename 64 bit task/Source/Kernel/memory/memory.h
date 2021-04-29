#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "./physical/pmm.h"
#include "./paging/paging.h"
#include "./heap/heap.h"
#include "stdint.h"

void memcpy(unsigned char *dest, const unsigned char *src, unsigned char len);

void memset(unsigned char *dest, unsigned char val, unsigned char len);

void memmove(unsigned char *dest, unsigned char *src, unsigned char len);

int memcmp(const unsigned char *src1, const unsigned char *src2, unsigned int len);


typedef struct {

	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t extended_type;


}__attribute__((packed)) memory_map_entry_t;

typedef struct {

	uint32_t reserved0;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t reserved1;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t reserved2;
	uint32_t reserved3;
	uint32_t iobp;
	

}__attribute__((packed)) tss_t;


tss_t systemTSS;
memory_map_entry_t* currentMemoryMap;
uint64_t getTotalMemory(memory_map_entry_t * map);

uint64_t getTotalUsableMemory(memory_map_entry_t * map);

#endif