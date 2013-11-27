/*
 * Matthew Tay mhtay@andrew.cmu.edu	
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 */
#include <bits/swi.h> 
#include <assert.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <arm/reg.h>
#include <types.h>
#include <task.h>
#include <syscall.h>
#include <lock.h>





/* Called by assembly Swi_Handler, with a swi number and a pointer to
 * register values on the stack. 
 * requires valid swi_Num, this check is done in kernel.
 * returns a return value, depending on which swi_handler was called.
 */
int C_SWI_Handler(int swi_Num, unsigned int *regs) {
	switch( swi_Num ) {
		case READ_SWI: return read_syscall((int)regs[0], (char *)regs[1], (size_t)regs[2]);
		case WRITE_SWI: return write_syscall((int)regs[0], (char *)regs[1], (size_t)regs[2]);
		case TIME_SWI: return time_syscall();
		case SLEEP_SWI: 
			sleep_syscall((unsigned long)regs[0]);
			return 0;
		case CREATE_SWI: return task_create((task_t *)regs[0], (size_t)regs[1]);
		case MUTEX_CREATE: return mutex_create();
		case MUTEX_LOCK: return mutex_lock((int)regs[0]);
		case MUTEX_UNLOCK: return mutex_unlock((int)regs[0]);
		case EVENT_WAIT: return event_wait((unsigned int)regs[0]);
		default:
			invalid_syscall(swi_Num);
	}

	assert(0); //Should not reach here
	return(0);
}

