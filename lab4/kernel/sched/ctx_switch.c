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
	disable_interrupts(); 
    	printf("dispatch_save \n");

	unsigned linkReg = (unsigned) cur_tcb->context.lr; //check if first time run
	unsigned prio = highest_prio();
	unsigned cur_prio;
	
	cur_tcb = &system_tcb[prio];
	if (linkReg == 0) {
		cur_prio = get_cur_prio();
		// set new cur_tcb 
		cur_tcb = &system_tcb[prio];
		ctx_switch_full(&system_tcb[prio].context, &system_tcb[cur_prio].context);
	}
	else {
		// set new cur_tcb 
		cur_tcb = &system_tcb[prio];
		ctx_switch_half(&system_tcb[prio].context);
		launch_task();
	}
	enable_interrupts();

//    tcb_t *tmp = cur_tcb;
//   tcb_t *hp = runqueue_remove(highest_prio());
//   runqueue_add(cur_tcb,cur_tcb->native_prio);
//    cur_tcb = hp; 
//    ctx_switch_full(&(hp->context),&(tmp->context)) ;
//    enable_interrupts();
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{	
	disable_interrupts(); 
	printf("dispatch_nosave\n");
	unsigned prio = highest_prio();
	printf("prio = %d\n",prio);
	unsigned cur_prio = cur_tcb->native_prio;
	printf("system_tcb[1]'s context value %d\n", system_tcb[1].context.r4);
	

	if (prio == cur_prio) {
		//Find next highest priority if current task is highest priority - not likely 
		// to be used (for now in debug) so return;				
		printf("PRIO== cur_prio of %d ?\n", (int) cur_prio);
		panic("hi");
		if (cur_prio != IDLE_PRIO) {
			runqueue_remove(cur_prio);
			prio = highest_prio();
			runqueue_add(cur_tcb, cur_prio);
			// set new current tcb 
			cur_tcb = &system_tcb[prio];
			ctx_switch_half(&(system_tcb[prio].context));
		}
		else {
			cur_tcb = &system_tcb[prio];
			// cur_prio == prio == IDLE_PRIO 
			ctx_switch_half(&(cur_tcb->context));
		}
	}
	
	else {
		// set new current tcb 
		cur_tcb = &system_tcb[prio];
		printf("new tcb has prio: %d\n", system_tcb[prio].native_prio);
		ctx_switch_half(&(system_tcb[prio].context));
	}

}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{	
	printf("dispatch_sleep\n");
	disable_interrupts(); 

	unsigned prio = highest_prio();
	unsigned cur_prio = cur_tcb->native_prio;
	
	

	if (cur_prio == IDLE_PRIO) {
		printf("should not be calling dispatch sleep from idle task!\n");
		panic("hi");
	}
	//Find next highest priority
	runqueue_remove(cur_prio);
	prio = highest_prio();
	printf("new highest prio is %d\n", prio);

	
	// set global cur_tcb 
	cur_tcb = &system_tcb[prio];
	printf("system_tcb value of context's lr is %x\n",(int) system_tcb[prio].context.lr);
	printf("system_tcb value of context's r4 is %x\n",(int) system_tcb[prio].context.r4);
	printf("system_tcb value of context's r5 is %x\n",(int) system_tcb[prio].context.r5);
	printf("cur prio = %d prio = %d\n", cur_prio, prio);
	// interrupts enabled in ctx_switch
	ctx_switch_full(&(system_tcb[prio].context), &(system_tcb[cur_prio].context)); /// reached 
	
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
