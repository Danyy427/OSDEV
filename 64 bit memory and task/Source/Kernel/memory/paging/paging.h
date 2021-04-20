#ifndef _PAGING_H_
#define _PAGING_H_


void initPaging();

extern void enablePaging(unsigned int*);


typedef struct
{
	unsigned long present                   : 1;
    unsigned long writeable                 : 1;
    unsigned long user_access               : 1;
    unsigned long write_through             : 1;
    unsigned long cache_disabled            : 1;
    unsigned long accessed                  : 1;
	unsigned long dirty                     : 1;
	unsigned long zero                      : 1;
    unsigned long global                    : 1;
    unsigned long ignored_2                 : 3;
	unsigned long address                   : 28;
    unsigned long reserved_1                : 12;
    unsigned long ignored_1                 : 11;
    unsigned long execution_disabled        : 1;
} __attribute__((packed)) page_table_entry_t;

typedef struct {
	page_table_entry_t entries[512];
} __attribute__((packed)) page_table_t;

typedef struct
{
	unsigned long present                   : 1;
    unsigned long writeable                 : 1;
    unsigned long user_access               : 1;
    unsigned long write_through             : 1;
    unsigned long cache_disabled            : 1;
    unsigned long accessed                  : 1;
	unsigned long zero                      : 1;
    unsigned long size                      : 1;
    unsigned long global                    : 1;
    unsigned long ignored_2                 : 3;
	unsigned long address                   : 28;
    unsigned long reserved_1                : 12;
    unsigned long ignored_1                 : 11;
    unsigned long execution_disabled        : 1;
} __attribute__((packed)) page_directory_entry_t;

typedef struct 
{
	page_directory_entry_t entries[512];

} __attribute__((packed)) page_directory_t;

typedef struct
{
	unsigned long present                   : 1;
    unsigned long writeable                 : 1;
    unsigned long user_access               : 1;
    unsigned long write_through             : 1;
    unsigned long cache_disabled            : 1;
    unsigned long accessed                  : 1;
	unsigned long zero                      : 1;
    unsigned long size                      : 1;
    unsigned long global                    : 1;
    unsigned long ignored_2                 : 3;
	unsigned long address                   : 28;
    unsigned long reserved_1                : 12;
    unsigned long ignored_1                 : 11;
    unsigned long execution_disabled        : 1;
} __attribute__((packed)) page_directory_pointer_table_entry_t;

typedef struct 
{
	page_directory_pointer_table_entry_t entries[512];

}  __attribute__((packed))page_directory_pointer_table_t;

typedef struct
{
	unsigned long present                   : 1;
    unsigned long writeable                 : 1;
    unsigned long user_access               : 1;
    unsigned long write_through             : 1;
    unsigned long cache_disabled            : 1;
    unsigned long accessed                  : 1;
	unsigned long zero                      : 1;
    unsigned long size                      : 1;
    unsigned long global                    : 1;
    unsigned long ignored_2                 : 3;
	unsigned long address                   : 28;
    unsigned long reserved_1                : 12;
    unsigned long ignored_1                 : 11;
    unsigned long execution_disabled        : 1;
} __attribute__((packed)) pml4_entry_t;

typedef struct 
{
	pml4_entry_t entries[512];

} __attribute__((packed)) pml4_t;


pml4_t *currentPML4;

void map_page(pml4_t *pml4, void * physaddr, unsigned long  virtualaddr, unsigned int flags);

void * get_physaddr(pml4_t *pml4, unsigned long virtualaddr);

void setAsCurrentPML4(pml4_t *pml4);
pml4_t* getCurrentPML4();

#endif