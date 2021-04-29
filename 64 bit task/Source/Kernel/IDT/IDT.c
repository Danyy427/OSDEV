
#include "IDT.h"

#define PUSHALL \
	 ".intel_syntax noprefix\n\t" \
    "push    r15\n\t"\
    "push    r14\n\t"\
    "push    r13\n\t"\
    "push    r12\n\t"\
    "push    r11\n\t"\
    "push    r10\n\t"\
    "push    r9\n\t"\
    "push    r8\n\t"\
    "push    rax\n\t"\
    "push    rcx\n\t"\
    "push    rdx\n\t"\
    "push    rbx\n\t"\
    "push    rsp\n\t"\
    "push    rbp\n\t"\
    "push    rsi\n\t"\
    "push    rdi\n\t"\
	 ".att_syntax prefix\n\t" \

#define POPALL\
	".intel_syntax noprefix\n\t" \
    "pop     rdi\n\t"\
    "pop     rsi\n\t"\
    "pop     rbp\n\t"\
    "add     rsp, 8\n\t"\
    "pop     rbx\n\t"\
    "pop     rdx\n\t"\
    "pop     rcx\n\t"\
    "pop     rax\n\t"\
    "pop     r8\n\t"\
    "pop     r9\n\t"\
    "pop     r10\n\t"\
    "pop     r11\n\t"\
    "pop     r12\n\t"\
    "pop     r13\n\t"\
    "pop     r14\n\t"\
    "pop     r15\n\t"\
    "add     rsp, 0x10\n\t"\
	".att_syntax prefix\n\t" \
	

/* Common body for interrupt handler */
#define MAKE_INTERRUPT_COMMON_STUB(intno, intfunc) \
			"push $"#intno"\n\t" \
			PUSHALL	 \
			"mov %rsp, %rcx\n\t"\
			"sub $0x28, %rsp\n\t"\
			"cld\n\t" \
			"call " #intfunc "\n\t" \
			"add $0x28, %rsp\n\t" \
			POPALL \
			"iretq \n\t" 

			
#define MAKE_IRQ_COMMON_STUB(intno, intfunc) \
			"push $"#intno"\n\t" \
			PUSHALL	 \
			"mov %rsp, %rcx\n\t"\
			"cld\n\t" \
			"call " #intfunc "\n\t" \
			POPALL \
			"iretq \n\t" 
/* Make interrupt for exception without error code. Push a dummy value for the
 * error code in it place. Push all the segment registers and the segment registers
 * so that they are available to interrupt function (intfun). Pushes a unique
 * interrupt number (intno) after the error code so that a handler can be multiplexed
 * if needed. Restore all the registers upon exit.
 *
 * intentry: Is the interrupt entry point that can be used in an Interrupt
 *           Descriptor Table (IDT) entry.
 * intfunc:  Is the C interrupt function that the stub calls to do processing
 * intno:    Interrupt number. Can be used to multiplex multiple interrupts to one
 *           intfunc handler.
 */
#define MAKE_INTERRUPT(intentry, intfunc, intno) \
	extern void intentry (void); \
	__asm__(".global " #intentry "\n\t" \
			".align 16\n\t" \
			#intentry ":\n\t" \
			"push $0\n\t"            /* Push dummy error code */ \
			MAKE_INTERRUPT_COMMON_STUB(intno, intfunc));
			
#define MAKE_IRQ(intentry, intfunc, intno) \
	extern void intentry (void); \
	__asm__(".global " #intentry "\n\t" \
			".align 16\n\t" \
			#intentry ":\n\t" \
			"push $0\n\t"            /* Push dummy error code */ \
			MAKE_IRQ_COMMON_STUB(intno, intfunc));

/* Make interrupt for exception with error code. Processor pushes the error code
 * after the return address automatically. Push all the segment registers and the
 * segment registers so that they are available to interrupt function (intfun).
 * Pushes a unique interrupt number (intno) after the error code so that a handler
 * can be multiplexed if needed. Restore all the registers upon exit.
 *
 * intentry: Is the interrupt entry point that can be used in an Interrupt
 *           Descriptor Table (IDT) entry.
 * intfunc:  Is the C interrupt function that the stub calls to do processing
 * intno:    Interrupt number. Can be used to multiplex multiple interrupts to one
 *           intfunc handler.
*/
#define MAKE_INTERRUPT_ERRCODE(intentry, intfunc, intno) \
	extern void intentry (void); \
	__asm__(".global " #intentry "\n" \
			".align 16\n\t" \
			#intentry ":\n\t" \
			MAKE_INTERRUPT_COMMON_STUB(intno, intfunc));

MAKE_INTERRUPT        (isr0,  isr0_handler,  0x00)
MAKE_INTERRUPT        (isr1,  isr1_handler,  0x01)
MAKE_INTERRUPT        (isr2,  isr2_handler,  0x02)
MAKE_INTERRUPT        (isr3,  isr3_handler,  0x03)
MAKE_INTERRUPT        (isr4,  isr4_handler,  0x04)
MAKE_INTERRUPT        (isr5,  isr5_handler,  0x05)
MAKE_INTERRUPT        (isr6,  isr6_handler,  0x06)
MAKE_INTERRUPT        (isr7,  isr7_handler,  0x07)
MAKE_INTERRUPT_ERRCODE(isr8,  isr8_handler,  0x08)
MAKE_INTERRUPT        (isr9,  isr9_handler,  0x09)
MAKE_INTERRUPT_ERRCODE(isr10, isr10_handler, 0x0a)
MAKE_INTERRUPT_ERRCODE(isr11, isr11_handler, 0x0b)
MAKE_INTERRUPT_ERRCODE(isr12, isr12_handler, 0x0c)
MAKE_INTERRUPT_ERRCODE(isr13, isr13_handler, 0x0d)
MAKE_INTERRUPT_ERRCODE(isr14, isr14_handler, 0x0e)
/* Reserved 0x0f */
MAKE_INTERRUPT        (isr16, isr0_handler,  0x10)
MAKE_INTERRUPT_ERRCODE(isr17, isr0_handler,  0x11)
MAKE_INTERRUPT        (isr18, isr0_handler,  0x12)
MAKE_INTERRUPT        (isr19, isr0_handler,  0x13)
MAKE_INTERRUPT        (isr20, isr0_handler,  0x14)
/* Reserved 0x15 to 0x1d */
MAKE_INTERRUPT_ERRCODE(isr30, isr0_handler,  0x1e)
/* Reserved 0x1f */

/* IRQ handlers */
MAKE_IRQ        (irq0,  irq0_handler,  0x0)
MAKE_IRQ        (irq1,  irq1_handler,  0x1)
MAKE_IRQ        (irq2,  irq2_handler,  0x2)
MAKE_IRQ        (irq3,  irq3_handler,  0x3)
MAKE_IRQ        (irq4,  irq4_handler,  0x4)
MAKE_IRQ        (irq5,  irq5_handler,  0x5)
MAKE_IRQ        (irq6,  irq6_handler,  0x6)
MAKE_IRQ        (irq7,  irq7_handler,  0x7)
MAKE_IRQ        (irq8,  irq8_handler,  0x8)
MAKE_IRQ        (irq9,  irq9_handler,  0x9)
MAKE_IRQ        (irq10, irq10_handler,  0xA)
MAKE_IRQ        (irq11, irq11_handler,  0xB)
MAKE_IRQ        (irq12, irq12_handler,  0xC)
MAKE_IRQ        (irq13, irq13_handler,  0xD)
MAKE_IRQ        (irq14, irq14_handler,  0xE)
MAKE_IRQ        (irq15, irq15_handler,  0xF)

void init_idt_entry(int num, unsigned int offset, unsigned short select, 
	unsigned short flags)
	{
		
		_idt_entries[num].offset0 = (unsigned short)(offset & 0x000000000000FFFF);
		_idt_entries[num].offset1 = (unsigned short)((offset & 0x00000000FFFF0000) >> 16);
		_idt_entries[num].offset2 = (unsigned int)((offset & 0xFFFFFFFF00000000) >> 32);

		_idt_entries[num].selector = select;
		_idt_entries[num].flags = flags;
		 return;
	}




///////////////////////////////////////
void idt_flush(struct idt *idtr)
{
	asm volatile("lidt %0" :: "m"(*idtr));
}
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
void init_pic()
{
	/*
	outb(0x20,0x11);
	outb(0xA0,0x11);

	outb(0x21, 0x20);
	outb(0xA1, 40);

	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	*/
	 unsigned char a1, a2;
    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2); 
}


void init_idt()
{
	
	
	_idt_entries = RequestPage();
	Tidt.limit = 16 * 256;
	Tidt.base  = _idt_entries;
	
	memset(_idt_entries, 0, 16*256);
	
	
	
	for(int i = 0; i < 256 ; i++){
		init_idt_entry(i,(int)&isr0,0x08, 0x8E); 
	}
	init_idt_entry(0,(int)&isr0,0x08, 0x8E);
	init_idt_entry(1,(int)&isr1,0x08, 0x8E);
	init_idt_entry(2,(int)&isr2,0x08, 0x8E);
	init_idt_entry(3,(int)&isr3,0x08, 0x8E);
	init_idt_entry(4,(int)&isr4,0x08, 0x8E);
	init_idt_entry(5,(int)&isr5,0x08, 0x8E);
	init_idt_entry(6,(int)&isr6,0x08, 0x8E);
	init_idt_entry(7,(int)&isr7,0x08, 0x8E);
	init_idt_entry(8,(int)&isr8,0x08, 0x8E);
	init_idt_entry(9,(int)&isr9,0x08, 0x8E);
	init_idt_entry(10,(int)&isr10,0x08, 0x8E);
	init_idt_entry(11,(int)&isr11,0x08, 0x8E);
	init_idt_entry(12,(int)&isr12,0x08, 0x8E);
	init_idt_entry(13,(int)&isr13,0x08, 0x8E);
	init_idt_entry(14,(int)&isr14,0x08, 0x8E);
	/* ISR15 is reserved */
	init_idt_entry(16,(int)&isr16,0x08, 0x8E);
	init_idt_entry(17,(int)&isr17,0x08, 0x8E);
	init_idt_entry(18,(int)&isr18,0x08, 0x8E);
	init_idt_entry(19,(int)&isr19,0x08, 0x8E);
	init_idt_entry(20,(int)&isr20,0x08, 0x8E);
	/* ISR21 to ISR2F are reserved */
	init_idt_entry(30,(int)&isr30,0x08, 0x8E);

	/* IRQ handlers */
	init_idt_entry(32,(int)&irq0,0x08, 0x8E);
	init_idt_entry(33,(int)&irq1,0x08, 0x8E);
	init_idt_entry(34,(int)&irq2,0x08, 0x8E);
	init_idt_entry(35,(int)&irq3,0x08, 0x8E);
	init_idt_entry(36,(int)&irq4,0x08, 0x8E);
	init_idt_entry(37,(int)&irq5,0x08, 0x8E);
	init_idt_entry(38,(int)&irq6,0x08, 0x8E);
	init_idt_entry(39,(int)&irq7,0x08, 0x8E);
	init_idt_entry(40,(int)&irq8,0x08, 0x8E);
	init_idt_entry(41,(int)&irq9,0x08, 0x8E);
	init_idt_entry(42,(int)&irq10,0x08, 0x8E);
	init_idt_entry(43,(int)&irq11,0x08, 0x8E);
	init_idt_entry(44,(int)&irq12,0x08, 0x8E);
	init_idt_entry(45,(int)&irq13,0x08, 0x8E);
	init_idt_entry(46,(int)&irq14,0x08, 0x8E);
	init_idt_entry(47,(int)&irq15,0x08, 0x8E);
	
	
	init_pic();
	
	SetPITSpeed(100);
	drawStringToCursor("Set PIT speed\n",0xffffff, 0x000000);		
	
	idt_flush(&Tidt);
	
	
	
	int s = initPS2Controller();
	if(s == 0) drawStringToCursor("Successfully initialized PS2 Controller\n",0xffffff, 0x000000);
	else drawStringToCursor("A problem occured while initializing PS2 Controller\n",0xffffff, 0x000000);
	s = 0;
	s = initKeyboard();
	if(s == 0xFA) drawStringToCursor("Successfully initialized Keyboard Controller\n",0xffffff, 0x000000);
	else drawStringToCursor("A problem occured while initializing Keyboard Controller\n",0xffffff, 0x000000);
	s = 0;
	s = initMouse();
	if(s == 0) drawStringToCursor("Successfully initialized Mouse Controller\n",0xffffff, 0x000000);
	else drawStringToCursor("A problem occured while initializing Mouse Controller\n",0xffffff, 0x000000);
	
	outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);
	
	//int d = 5 / 0;
	asm volatile("sti;");
	//while(1);
	
}