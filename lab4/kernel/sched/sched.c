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

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void scheduleTCBs(task_t *tasks, size_t num_tasks);


void sched_init(task_t* main_task  __attribute__((unused)))
{
	scheduleTCBs(main_task	
}

//**********************helper functions********************************

// sort from smallest completion time to largest 
void scheduleTCBs(task_t *tasks, size_t num_tasks) {
	for (int x=0;x<num_tasks;x++) {
		for (int y=0;y<num_tasks-1;y++) {
			if (tasks[y].C > tasks[y+1].C) {
				// swap 
				task_t temp = tasks[y];
				tasks[y] = tasks[y+1];
				tasks[y+1] = temp;
			}
		}
	}
}	







//**********************************************************************
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
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	
}

