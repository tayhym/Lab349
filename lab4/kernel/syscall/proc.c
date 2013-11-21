/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

void createTCBs(task_t *tasks, size_t num_tasks);
void scheduleTCBs(task_t *tasks, size_t num_tasks);
int findCompletionTime(task_t *tasks, size_t num_tasks);
int findShortestPeriod(task_t *tasks, size_t num_tasks);
int tasksNotSchedulable(task_t *tasks, size_t num_tasks);
int checkForError(task_t *tasks, size_t num_tasks);

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	int errorValue; 	
	// init empty run-queue 
	runqueue_init();
	// schedule TCBs 
	scheduleTCBs(tasks, num_tasks);
	// check for errors
	errorValue = checkForError(tasks, num_tasks);
	if (errorValue != 0) {
		return errorValue;
	}
	// create TCBs to house tasks 
	createTCBs(tasks, num_tasks);

  return 1; /* remove this line after adding your code */
}



// ************** helper functions ****************************
void createTCBs(task_t *tasks, size_t num_tasks) { 
	
	for (int i=0; i<num_tasks;i++) {
		// initialize empty context		
		sched_context_t tcbContext = {.r4 = 0, .r5 = 0, .r6 = 0, .r7 = 0, .r8 = 0, .r9 = 0, 
									  .r10 = 0, .r11 = 0, .sp = tasks[i].stack_pos,
									  .lr = tasks[i].lambda};
		// initialize empty tcp, without priority. 0 is reserved for king-for-a-day.
		tcb_t tcb = {.native_prio = (i+1), .cur_prio = (i+1), .context = tcbContext, 
					 .holds_lock = 0, .sleep_queue = null, .kstack =tasks[i].stack_pos,
					 .kstack_high = tasks[i].stack_pos};
		
		run_list[i] = tcb;  
	}
}


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

// checks for error. return 0 on no error. 	
int checkForError(task_t *tasks, size_t num_tasks) {
	// use 1 for king-for-a-day, as mentioned in lecture. 1 for idle task	
	if (num_tasks>62) {
		return EINVAL;
	}
	else if (!((tasks >= RAM_START_ADDR) && (tasks <= RAM_END_ADDR))) {
		return EFAULT;
	}
	else if (tasksNotSchedulable) {
		return ESCHED;	
	} 
	return 0;
}

// find if tasks can be scheduled
int tasksNotSchedulable(task_t *tasks, size_t num_tasks) {
	int sPeriod = findShortestPeriod(tasks, num_tasks);
	int cTime = findCompletionTime(tasks, num_tasks);
	
	// not enough time to run all tasks
	if (cTime > sPeriod) {
		return 1;
	}
	return 0;
}

// find shortest task period
int findShortestPeriod(task_t *tasks, size_t num_tasks) {
	int shortestPeriod;		
	for (int i=0;i<num_tasks;i++) {
		if ((i==0) || (shortestPeriod>tasks[i].T)) {
			shortestPeriod = tasks[i].T;
		}
	}
}

// find completion time for all tasks
int findCompletionTime(task_t *tasks, size_t num_tasks) {
	int cTime = 0;
	for (int i = 0;i<num_tasks;i++) {
		cTime += tasks[i].C;
	}
}

//*****************************************************************


int event_wait(unsigned int dev  __attribute__((unused)))
{
  return 1; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
