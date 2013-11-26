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
	//initialize all devices to default values
	for (i = 0; i < NUM_DEVICES; i++) {
		devices[i].sleep_queue = 0; //Empty sleep queue
		//Next match based on current time + dev freq
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
	disable_interrupts();
	printf("dev_wait\n");
	
	tcb_t *currSleepQueue = devices[dev].sleep_queue;

	while (currSleepQueue != 0) {
		currSleepQueue = currSleepQueue->sleep_queue;
	}

	currSleepQueue = get_cur_tcb();

	currSleepQueue->sleep_queue = 0;

	enable_interrupts();
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
	tcb_t* currTask;

    for (i = 0; i < NUM_DEVICES; i++) {
		// Check if time to wake up task from current device 
        if (devices[i].next_match > millis) {
			disable_interrupts();
			// Get next match time based on current time and dev freq
            devices[i].next_match = clock + dev_freq[i];
			
			// If dev sleep queue is empty, nothing to update
         	if (devices[i].sleep_queue != 0) {
		        for (currTask= devices[i].sleep_queue; currTask != 0; currTask=currTask->sleep_queue) {
					// wake up each task in dev and add to run_queue
		            runqueue_add(currTask, currTask->native_prio);
		        }
				// Reset current dev's sleep queue to empty
		        devices[i].sleep_queue = 0;
		            
				enable_interrupts();
		        dispatch_save();
			}
        }
    }

    enable_interrupts();
}

