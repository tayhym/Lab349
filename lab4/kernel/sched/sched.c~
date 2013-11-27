/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"
#include <device.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */
uint32_t idleStack[4*OS_KSTACK_SIZE/sizeof(uint32_t)];

void addIdleTask();
void makeTasksRunnable(size_t num_tasks);
void launchHighestPrio(void);

void sched_init(task_t* main_task  __attribute__((unused)))
{	// not used. 	
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given processes.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
` *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	printf("allocate_tasks\n");
	unsigned int i;
    runqueue_init();
    dev_init();

    for (i = 0; i < num_tasks; i++) {
	
		sched_context_t tcbContext = {.r4 = (unsigned)(tasks[i]->lambda), //user entry point
									  .r5 = (unsigned)(tasks[i]->data), //user argument 0
									  .r6 = (unsigned)(tasks[i]->stack_pos), //user sp
									  .r7 = 0, .r8 = 0, .r9 = 0, .r10 = 0, .r11 = 0,
									  .sp = (void *)(((char *)system_tcb[i].kstack)+OS_KSTACK_SIZE-4),
									  .lr = 0};
		
		system_tcb[i].native_prio = i;
		system_tcb[i].cur_prio = i;
		system_tcb[i].context = tcbContext;
		system_tcb[i].holds_lock = 0;
		system_tcb[i].sleep_queue = 0;
		system_tcb[i].kstack_high[0] = (uint32_t)system_tcb[i].kstack;
		//printf("system_tcb[%d].context.r4 = %d\n",i,system_tcb[i].context.r4);
	}

	addIdleTask();
	makeTasksRunnable(num_tasks);
	launchHighestPrio();
}


//*****************************helper function *****************************************


// launch highest priority task
void launchHighestPrio() {
	printf("launchHighestPrio\n");
	unsigned prio = highest_prio();

	/* extract r4 = user program entry point
	 *	  	   r5 = prog argument 0
	 *		   r6 = user stack pointer 
	 */			 
	printf("highest prio %d\n",(int) prio);	
	unsigned entryPoint = system_tcb[prio].context.r4;
	printf("entryPoint %x\n", (int) entryPoint);	
	unsigned progData = system_tcb[prio].context.r5;
	printf("progData %x\n", (int) progData); 
	unsigned userSp = system_tcb[prio].context.r6;
	printf("userSp %x\n", (int) userSp);
	
	/* dispatch to current priority group */
	dispatch_nosave();
}

// make tasks, other than idle task, runnable
void makeTasksRunnable(size_t num_tasks) {	
	size_t i;uint8_t prio;
	for (i = 0; i < num_tasks; i++) {
		prio = i;
		disable_interrupts();
		runqueue_add(&system_tcb[i], prio);	
		printf("added prio %d",prio);	
		enable_interrupts();	
	}		
}

// add Idle Task to systemTCB 
void addIdleTask() {
	printf("addIdleTask\n");

	unsigned IDLE_STACK_POS = RAM_END_ADDR;
	// Initialize empty context
	sched_context_t tcbContext = {.r4 = (unsigned) idle,  // lambda 
								  .r5 = 0, 	   // user argument 0
								  .r6 = IDLE_STACK_POS, // user sp 
								  .r7 = 0, .r8 = 0, .r9 = 0,  
								  .r10 = 0, .r11 = 0, 
								  .sp = (unsigned *) IDLE_STACK_POS, // current sp
							      .lr = 0x0            
								 };
	
	system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].context = tcbContext;
	system_tcb[IDLE_PRIO].holds_lock = 0;
	system_tcb[IDLE_PRIO].sleep_queue = (void *)0x0;
	system_tcb[IDLE_PRIO].kstack_high[0] =(uint32_t)system_tcb[IDLE_PRIO].kstack;

	// make idle task runnable	
	disable_interrupts();
	runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);

	// init dispatcher
	dispatch_init(&system_tcb[IDLE_PRIO]);
	enable_interrupts();
}














