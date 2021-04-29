#ifndef _TASK_H_
#define _TASK_H_

#include "../memory/memory.h"

typedef struct {

    uint64_t rdi, rsi, rbp, useless, rbx, rdx, rcx, rax;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t reserved1, reserved2;
    uint64_t rip, cs, rflags, rsp, ss;

} __attribute__((packed)) task_registers_t;

typedef struct task
{
	uint64_t PID;
	uint64_t entry;
	uint64_t privilege;
	uint64_t taskState;
	char * name;
	char * description;
	task_registers_t state;
	uint64_t kernel_stack;
	uint64_t user_stack;
	heap_t heap;
	pml4_t * pml4;
	struct task * next;
	
} __attribute__((packed)) task_t;


enum taskStates{

	NOTRUN = 0,
	RUNNING = 1,
	WAITING = 2,
	BLOCKED = 3,
	DELETE = 4
};

void initTasking();

task_t *taskQueue;
task_t *currentTask;


int isTasking;
extern void setStack(uint64_t newStack);
extern void PUSH_64(uint64_t val);
extern void pushRegisters(task_registers_t* regs);
extern uint64_t read_rip(void);

void switch_task(/*task_t *task*/);
void printRegisters(task_registers_t *regs);
#endif