/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#include <arm/exception.h>

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	cur_tcb = idle;
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{	
	// disable interrupts - if first time run task, then launch task. else, dispatch no 		save.
	disable_interrupts();
	
	unsigned prio  = highest_prio();
	unsigned cur_prio = get_cur_prio();
	ctx_switch_full(&system_tcb[prio].context, &system_tcb[cur_prio].context);
	// now in new context, set cur_tcb 	
	cur_tcb = &system_tcb[prio];
	
	//enable interrupts
	enable_interrupts();
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{	
	// disable interrupts

	// temporarily remove current task, to find next highest priority
	if (cur_tcb->native_prio != IDLE_PRIO) {
		runqueue_remove(cur_tcb->native_prio);
	}
	unsigned prio  = highest_prio();
	// add back current task	
	if (cur_tcb->native_prio != IDLE_PRIO) {
		runqueue_add(cur_tcb, cur_tcb->native_prio);
	} 	
	// context switch
	ctx_switch_half(&system_tcb[prio].context);
	
	// now in new context, set cur_tcb
	cur_tcb = &system_tcb[prio];

	// enable interrupts
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	// remove current task from run-bits, if not idle 
	if (cur_tcb->native_prio != IDLE_PRIO) {
		runqueue_remove(cur_tcb->native_prio);
	} 
	unsigned prio  = highest_prio();
	unsigned cur_prio = get_cur_prio();
	ctx_switch_full(&system_tcb[prio].context, &system_tcb[cur_prio].context);
	
	// now in new context, set cur_tcb
	cur_tcb = &system_tcb[prio];	
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->native_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{	
	// global value
	return cur_tcb;
}
