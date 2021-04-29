
#include "../video/video.h"
#include "../string/string.h"
#include "../IO/io.h"
#include "./isr/isr.h"
#include "./irq/irq.h"
#include "../memory/memory.h"
#include "stdint.h"
#ifndef _IDT_H_
#define _IDT_H_




#define IDTBASE    0x00000000
#define IDTSIZE 0xFF

int EnvironmentTick;

typedef struct idt
{	
	unsigned short limit;
	uint64_t base;
}__attribute__ ((packed)) idt_t;

typedef struct idt_entry
{
	/*
	short base_low;
    short select;
    char always0;
    char flags;
    short base_high;
	*/
	unsigned short offset0;
    unsigned short selector;
    unsigned char ist;
    unsigned char flags;
    unsigned short offset1;
    unsigned int offset2;
    unsigned int ignore;
}__attribute__ ((packed)) idt_entry_t;

typedef struct {
    uint64_t rdi, rsi, rbp, useless, rbx, rdx, rcx, rax;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, rsp, ss;
}__attribute__((packed)) interrupt_frame_t;

idt_t Tidt;
idt_entry_t *_idt_entries;

void init_idt_entry(int num, unsigned int offset, unsigned short select, 
        unsigned short flags);

void idt_flush(struct idt *idtr);

void init_pic();

void init_idt();


#endif