#ifndef _GDT_H_
#define _GDT_H_
#include "../memory.h"


typedef struct {

    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high;
    uint8_t base_high;
} __attribute__((packed)) gdt_gate_t;


typedef struct {
	uint16_t limit;
	gdt_gate_t* base;
} __attribute__((packed)) gdt_t;


#endif