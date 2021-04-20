#include "isr.h"
#include "stdint.h"
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
	panic("Invalid Opcode");
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
	panic("General Protection Fault");
	
	
	while(1);
	//fillRect(0, 0, 1080, 1920, 0xff0000);
}
void isr14_handler(interrupt_frame_t *frame)
{
	panic("Page Fault");
	
	
   //fillRect(0, 0, 1080, 1920, 0xff0000);
   while(1);
}