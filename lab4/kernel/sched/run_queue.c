/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"



static tcb_t *run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{	
	int i;
	// makes tcb(s) point to null
	for (i=0; i<(OS_MAX_TASKS);i++) {		
		run_list[i] = (void *)0x0;
	}
	// make run-bits all 0
	for (i = 0; i<(OS_MAX_TASKS/8);i++) {
		run_bits[i] = 0;
	}	
	// make group-run-bits 0
	group_run_bits = 0;

	
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t *tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{
	int ostcby; unsigned ostcby_bit;
	int ostcbx; unsigned ostcbx_bit;
	// add tcb to run_queue
	run_list[prio] = tcb;

	// set group_run_bits and run_bits
	ostcby = (prio>>3);
	ostcbx = (prio & 0x7);
	
	ostcby_bit = 0x1 << ostcby;
	group_run_bits = group_run_bits | ostcby_bit;
	
	ostcbx_bit = 0x1 << ostcbx;
	run_bits[ostcby] = run_bits[ostcby] | ostcbx_bit;

}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t *runqueue_remove(uint8_t prio  __attribute__((unused)))
{
	// get tcb at given priority
	tcb_t *tcb = run_list[prio];
	// set tcb at prio to empty tcb
	tcb_t *tcbEmpty = (tcb_t *)0x0;
	run_list[prio] = tcbEmpty;
	
	// remove run_bits and group_run_bits
	int ostcby = (prio>>3);
	int ostcbx = (prio & 0x7);
	
	unsigned ostcby_bit = 0x1 << ostcby;

	
	unsigned ostcbx_bit = 0x1 << ostcbx;
	run_bits[ostcby] = run_bits[ostcby] & (~ostcbx_bit);	
	/* remove the group run bits if no more tasks in that group running */
	if (run_bits[ostcby] == 0) {
		group_run_bits = group_run_bits & (~ostcby_bit);
	}
	printf("REMOVED PRIO %d, new run_bits[0] = %x\n", prio, run_bits[ostcby]);	
	return tcb;
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
	printf("group_run_bits = %x\n", group_run_bits);
	
	unsigned y = prio_unmap_table[group_run_bits];
	unsigned x = prio_unmap_table[run_bits[y]];
	printf("prio_unmap_table x = %d\n", (int) prio_unmap_table[run_bits[y]]);
	uint8_t prio = (y<<3) + x;
	return prio;		
}









