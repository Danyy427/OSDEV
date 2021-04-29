#include "gdt.h"

extern __gdt_flush(gdt_t* gdt);

gdt_gate_t gates[7];

void set_gdt_gate(uint8_t number, uint64_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gates[number].base_low = (base & 0xFFFF);
    gates[number].base_middle = (base >> 16) & 0xFF;
    gates[number].base_high = (base >> 24) & 0xFF;

    gates[number].limit_low = (limit & 0xFFFF);
    gates[number].limit_high = (limit >> 16) & 0x0F;
    
    gates[number].limit_high |= granularity & 0xF0;
    gates[number].access = access;
}




void initGDT()
{	
	gdt_t gdt;
	gdt.limit = sizeof(gdt_gate_t) * 8-1 ;
	gdt.base = gates;
 
	
	
	set_gdt_gate(0, 0, 0x0, 0, 0);                              // 0
	set_gdt_gate(1, 0, 0xFFFFF, 0x9A, 0xA0);                        // 8
	set_gdt_gate(2, 0, 0xFFFFF, 0x92, 0xA0);                        // 10   
	set_gdt_gate(3, 0, 0xFFFFF, 0, 0);                              // 18
	set_gdt_gate(4, 0, 0xFFFFF, 0xF2, 0xA0);                        // 20
	set_gdt_gate(5, 0, 0xFFFFF, 0xFA, 0xA0);                        // 28
	set_gdt_gate(6, &systemTSS, &systemTSS+sizeof(tss_t), 0x89, 0x00);  // 30
	
	__gdt_flush(&gdt);
	
	
	
	memset(&systemTSS, 0, sizeof(tss_t));
	
	systemTSS.rsp0 = 0x90000;
	systemTSS.iobp = sizeof(tss_t);
	asm ("ltr %0" :: "r" ((uint16_t)0x30));
	
}


