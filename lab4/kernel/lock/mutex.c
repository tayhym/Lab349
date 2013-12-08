/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];
int num_mutex = 0;

void mutex_init()
{
	/*Initialize mutex*/
    mutex_t *mutex = &(gtMutex[num_mutex]);
    mutex->bAvailable = 1;   //Init to available state
    mutex->pHolding_Tcb = 0; //Nothing is holding mutex upon init
    mutex->bLock = 0;		 //Init to unlocked state
    mutex->pSleep_queue = 0; //Nothing is waitinh for mutex upon init
	
	num_mutex++;
}

int mutex_create(void)
{
	disable_interrupts();

	/* Check for maximum number of mutices */
	if (num_mutex == OS_NUM_MUTEX) {
		enable_interrupts();
		return -ENOMEM;
	}
	/* Otherwise init new mutex and add to array */
	mutex_init();

	enable_interrupts();
	return (num_mutex-1);
}

int mutex_lock(int mutex  __attribute__((unused)))
{	
	disable_interrupts();

	/* Check if mutex id has been initialized */
	if (mutex >= num_mutex) {
		enable_interrupts();
		return -EINVAL;
	}

	tcb_t *currTask = get_cur_tcb();
	mutex_t *currMutex = &(gtMutex[mutex]);
	/* Check if current task already holds lock*/
	if (currTask == currMutex->pHolding_Tcb) {
		enable_interrupts();
		return -EDEADLOCK;
	}

	cur_tcb tcbCurrent = get_cur_tcb();
	tcbCurrent->cur_prio = 0;

	currMutex->bAvailable = 0;
	currMutex->bLock = 1;

	enable_interrupts();
	return 0; //Return 0 to indicate success
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	disable_interrupts();

	/* Check if mutex has been initialized */
	if (mutex >= num_mutex) {
		enable_interrupts();
		return -EINVAL;
	}

	tcb_t *currTask = get_cur_tcb();
	mutex_t *currMutex = &(gtMutex[mutex]);
	/* Check if current task does not holds lock*/
	if (currTask != currMutex->pHolding_Tcb) {
		enable_interrupts();
		return -EPERM;
	}

	enable_interrupts();
	return 0; //Return 0 to indicate success
}

