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

double findUtilization(task_t *tasks, size_t num_tasks);
int tasksNotSchedulable(task_t *tasks, size_t num_tasks);
int checkForError(task_t *tasks, size_t num_tasks);
void scheduleTasks(task_t *tasks, size_t num_tasks);


int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	int errorValue;	
	// init empty run-queue 
	runqueue_init();
	// schedule TCBs 
	scheduleTasks(tasks, num_tasks);
	// check for errors
	errorValue = checkForError(tasks, num_tasks);
	if (errorValue != 0) {
		return errorValue;
	}
	// create TCBs to house tasks and make runnable 
	allocate_tasks(&tasks, num_tasks);
	
	
	/* launch highest priority task */
	//launchHighestPriority();	

  	return 1; /* remove this line after adding your code */
}



// ************** helper functions ****************************

//void launchHighestPriority() { 
//	unsigned prio = highestPrio();	
//}

// sort from smallest completion time to largest 
void scheduleTasks(task_t *tasks, size_t num_tasks) {
	size_t x; size_t y;
	for (x=0;x<num_tasks;x++) {
		for (y=0;y<num_tasks-1;y++) {
			if (tasks[y].T > tasks[y+1].T) {
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
	// use 0 for king-for-a-day, as mentioned in lecture. 63 for idle task	
	if (num_tasks>62) {
		return EINVAL;
	}
	else if (!(((unsigned) tasks >= RAM_START_ADDR) && ((unsigned)tasks <= RAM_END_ADDR))) {
		return EFAULT;
	}
	else if (tasksNotSchedulable(tasks, num_tasks) == 1) {
		return ESCHED;	
	} 
	return 0;
}

// find if tasks can be scheduled - 1 if not schedulable
int tasksNotSchedulable(task_t *tasks, size_t num_tasks) {
	double utilization = findUtilization(tasks, num_tasks);
	double UB_BOUND = 0.693;
	if (utilization< UB_BOUND) {
		return 0; 
	}
	return 1;
}


// find total utilization 
double findUtilization(task_t *tasks, size_t num_tasks) {
	size_t i;
	double utilization = 0.0; 		
	for (i=0;i<num_tasks;i++) {
		double period = tasks[i].T;
		double util = tasks[i].C/period;
		utilization += util;
	}
	return utilization;
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
