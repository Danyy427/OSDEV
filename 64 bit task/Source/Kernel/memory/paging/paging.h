#ifndef _PAGING_H_
#define _PAGING_H_

#include "stdint.h"
void initPaging();

extern void enablePaging(unsigned int*);


typedef struct
{
	uint64_t present                   : 1;
    uint64_t writeable                 : 1;
    uint64_t user_access               : 1;
    uint64_t write_through             : 1;
    uint64_t cache_disabled            : 1;
    uint64_t accessed                  : 1;
	uint64_t dirty                     : 1;
	uint64_t zero                      : 1;
    uint64_t global                    : 1;
    uint64_t ignored_2                 : 3;
	uint64_t address                   : 28;
    uint64_t reserved_1                : 12;
    uint64_t ignored_1                 : 11;
    uint64_t execution_disabled        : 1;
} __attribute__((packed)) page_table_entry_t;

typedef struct {
	page_table_entry_t entries[512];
} __attribute__((packed)) page_table_t;

typedef struct
{
	uint64_t present                   : 1;
    uint64_t writeable                 : 1;
    uint64_t user_access               : 1;
    uint64_t write_through             : 1;
    uint64_t cache_disabled            : 1;
    uint64_t accessed                  : 1;
	uint64_t zero                      : 1;
    uint64_t size                      : 1;
    uint64_t global                    : 1;
    uint64_t ignored_2                 : 3;
	uint64_t address                   : 28;
    uint64_t reserved_1                : 12;
    uint64_t ignored_1                 : 11;
    uint64_t execution_disabled        : 1;
} __attribute__((packed)) page_directory_entry_t;

typedef struct 
{
	page_directory_entry_t entries[512];

} __attribute__((packed)) page_directory_t;

typedef struct
{
	uint64_t present                   : 1;
    uint64_t writeable                 : 1;
    uint64_t user_access               : 1;
    uint64_t write_through             : 1;
    uint64_t cache_disabled            : 1;
    uint64_t accessed                  : 1;
	uint64_t zero                      : 1;
    uint64_t size                      : 1;
    uint64_t global                    : 1;
    uint64_t ignored_2                 : 3;
	uint64_t address                   : 28;
    uint64_t reserved_1                : 12;
    uint64_t ignored_1                 : 11;
    uint64_t execution_disabled        : 1;
} __attribute__((packed)) page_directory_pointer_table_entry_t;

typedef struct 
{
	page_directory_pointer_table_entry_t entries[512];

}  __attribute__((packed))page_directory_pointer_table_t;

typedef struct
{
	uint64_t present                   : 1;
    uint64_t writeable                 : 1;
    uint64_t user_access               : 1;
    uint64_t write_through             : 1;
    uint64_t cache_disabled            : 1;
    uint64_t accessed                  : 1;
	uint64_t zero                      : 1;
    uint64_t size                      : 1;	
    uint64_t global                    : 1;
    uint64_t ignored_2                 : 3;
	uint64_t address                   : 28;
    uint64_t reserved_1                : 12;
    uint64_t ignored_1                 : 11;
    uint64_t execution_disabled        : 1;
} __attribute__((packed)) pml4_entry_t;

typedef struct 
{
	pml4_entry_t entries[512];

} __attribute__((packed)) pml4_t;


pml4_t *currentPML4;

void map_page(pml4_t *pml4, void * physaddr,  unsigned long long virtualaddr, unsigned int flags);

void * get_physaddr(pml4_t *pml4, uint64_t virtualaddr);

void setAsCurrentPML4(pml4_t *pml4);
pml4_t* getCurrentPML4();

#endif