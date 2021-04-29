#include "task.h"

void printRegisters(task_registers_t *regs)
{
	printk("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x", regs->rdi, regs->rsi, regs->rbp, regs->rbx, regs->rdx, regs->rcx, regs->rax, regs->r8, regs->r9, regs->r10, regs->r11, regs->r12, regs->r13, regs->r14, regs->r15, regs->rip, regs->cs, regs->rflags, regs->rsp, regs->ss);

}


void exit (uint64_t ret)
{
	currentTask->taskState = DELETE;
	currentTask->state.rax = ret;
	while(1);
}




void schedule(task_registers_t *currentState)
{
	if(isTasking == 1)
	{
		
		if(/*currentTask->next != 0 &&*/ currentTask->taskState == RUNNING)
		{
			
			currentTask->taskState = WAITING;                            // make the current process waiting
			memcpy(&currentTask->state, currentState, sizeof(task_registers_t)); // save the state
			task_t* nextTask;
			if(currentTask->next != 0)
				nextTask = currentTask->next;
			else
				nextTask = taskQueue;
			
			if(nextTask->taskState == NOTRUN)               // if it has never been run before
			{	
				nextTask->taskState = RUNNING;              // lets run it
				nextTask->state.ss = currentTask->state.ss;
				nextTask->state.cs = currentTask->state.cs;
				nextTask->state.rflags = currentTask->state.rflags;
				nextTask->state.rsp = currentTask->state.rsp;
				nextTask->state.rbp = currentTask->state.rbp;
				nextTask->state.rip = nextTask->entry;     // get the next tasks states rip to be its entry
				memcpy(currentState, &nextTask->state, sizeof(task_registers_t));  // write it
				
				//printk(" A process wants to be run : %s", nextTask->name);
				
				
				currentTask = nextTask;
				
				return;
			}
			else if(nextTask->taskState == WAITING)         // if it is waiting to be run
			{ 
				
				nextTask->taskState = RUNNING;              // lets run it
				//printk(" A process wants to be running : %s %x %x         ", nextTask->name, nextTask->state->rip, currentTask->state->rip);
				
				memcpy(currentState, &nextTask->state, sizeof(task_registers_t));  // copy its state and put it into frame
				//printRegisters(nextTask->state);
				currentTask = nextTask;
				return;
			}
			else if(nextTask->taskState == DELETE || nextTask->taskState == BLOCKED)
			{
			
				currentTask = nextTask;
				return;
			}
		}
	}
	else
		return;

}

int task1main()
{
	while(1) {printk("b"); for(int i = 0; i<1000000; i++);}

}

void idle()
{
	while(1) asm("hlt");
}

void initTasking()
{
	task_t *newTask = kalloc(&currentHeap, sizeof(task_t));
	
	task_t *kernelTask = kalloc(&currentHeap, sizeof(task_t));
	
	task_t *idleTask = kalloc(&currentHeap, sizeof(task_t));
	
	//printk("%x %x", newTask, kernelTask);
	
	newTask->PID = 2;
	newTask->entry = &task1main;
	newTask->privilege = 0;
	newTask->name = "TestTask";
	newTask->taskState = NOTRUN;
	newTask->description = "Test";
	newTask->next = 0;
	newTask->user_stack = RequestPage();
	
	
	idleTask->PID = 1;
	idleTask->entry = &idle;
	idleTask->privilege = 0;
	idleTask->name = "Idle";
	idleTask->taskState = NOTRUN;
	idleTask->description = "Idle";
	idleTask->next = newTask;
	idleTask->user_stack = RequestPage();
	
	kernelTask->PID = 0;
	kernelTask->entry = 0x10000;
	kernelTask->privilege = 0;
	kernelTask->name = "Kernel";
	kernelTask->taskState = RUNNING;
	kernelTask->description = "Kernel";
	kernelTask->next = idleTask;
	kernelTask->user_stack = 0x90000;
	
	currentTask = kernelTask;
	taskQueue = kernelTask;
	
	
	
	
	isTasking = 1;
	
}




