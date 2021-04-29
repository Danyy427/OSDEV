#include "isr.h"
#include "stdint.h"


#include "../../task/task.h"
void isr0_handler(interrupt_frame_t *frame)
{
	panic("Divide by zero");
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr1_handler(interrupt_frame_t *frame)
{
	panic("Debug");
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr2_handler(interrupt_frame_t *frame)
{
	panic("NMI");
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr3_handler(interrupt_frame_t *frame)
{
	panic("Breakpoint");
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr4_handler(interrupt_frame_t *frame)
{
	panic("Overflow");
	while(1);
   //fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr5_handler(interrupt_frame_t *frame)
{
	panic("Bound range exceeded");
	while(1);
   //fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr6_handler(interrupt_frame_t *frame)
{
	panic("Invalid Opcode : ");
	//printk("%x\n", frame->rip);
	printRegisters((task_registers_t*)frame);
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr7_handler(interrupt_frame_t *frame)
{
	panic("Device Not Available");
	while(1);
   //fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr8_handler(interrupt_frame_t *frame)
{
	panic("Double Fault");
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr9_handler(interrupt_frame_t *frame)
{
	panic("COPRO SEGMENT OVERRUN");
	
	while(1);
   //fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr10_handler(interrupt_frame_t *frame)
{
	panic("Invalid TSS");
	
	while(1);
   //fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr11_handler(interrupt_frame_t *frame)
{
	panic("Segment Not present");
	
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr12_handler(interrupt_frame_t *frame)
{
	panic("Stack Segment Fault");
	
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr13_handler(interrupt_frame_t *frame)
{
	panic("General Protection Fault: Faulty Entry: ");
	
	printk("%x\n", frame->err_code);
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr14_handler(interrupt_frame_t *frame)
{
	uint64_t cr2;
    asm volatile("mov %%cr2, %0" : "=r" (cr2));
	
	//printk("Page Fault: Fault address: %x", cr2);
	uint32_t flags = frame->err_code;

    int present = !(flags & 0x1);
    int rw = flags & 0x2;
    int us = flags & 0x4;
    int reserved = flags & 0x8;
    int inst = flags & 0x10;

    printk("\n\n\n---------------------------------------------");

    printk("\n\n\nPAGE FAULT: at virtual address %x\n", cr2);

    printk("Flags:\n");
    printk("Present: %d\n", present);
    printk("Write: %d\n", rw);
    printk("User access: %d\n", us);
    printk("Reserved: %d\n", reserved);
    printk("Instruction fetch: %d\n", inst);
	printk("RIP: %x", frame->rip);
    printk("\n\n\n---------------------------------------------\n\n\n");

	
   //fillRect(0, 0, 1080, 1920, 0xff0000);
   while(1);
}