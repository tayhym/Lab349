/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

extern volatile unsigned long clock;

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];


/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
   	int i;
	for (i = (NUM_DEVICES-1); i > 0; i--) {
		devices[i].sleep_queue = 0;
		devices[i].next_match = clock + dev_freq[i];
	}
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
    tcb_t* sq = devices[dev].sleep_queue;
	/* Check for empty sleep queue */
    if (sq == 0) {
       devices[dev].sleep_queue = get_cur_tcb();
       devices[dev].sleep_queue->sleep_queue = 0;
    } 

    else {
       while (sq->sleep_queue != 0) 
            sq = sq->sleep_queue;
       sq->sleep_queue = get_cur_tcb();
       sq = sq->sleep_queue;
       sq->sleep_queue = 0;
    }

    dispatch_sleep();
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused)))
{
	int i;
    for (i = (NUM_DEVICES-1); i > 0; i--) {
        if (devices[i].next_match == millis) {
            devices[i].next_match += dev_freq[i];
            tcb_t* sq = devices[i].sleep_queue;
            if (sq != 0) {
                while(sq != 0) {
                    runqueue_add(sq,sq->native_prio);
                    sq = sq->sleep_queue;
                }
                devices[i].sleep_queue = 0;
                
				enable_interrupts();
                dispatch_save();   // Switch contextes if things added to runqueue
            }
        }
    }
}

