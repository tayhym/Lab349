/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif
#include <math.h>
 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	unsigned i;
	double C, T;
	double sum = 0;

	for (i = 0; i < num_tasks; i++) { 
		C = (double)(tasks[i]->C);
		T = (double)(tasks[i]->T);
		if (T == 0) {
			return 0;
		}
		sum += C/T;
		if (i == (num_tasks-1)) {
			sum += ((double)(tasks[i]->B))/T;
		}
	}

	if ((num_tasks*ilog2(sum+num_tasks)) <= (double)(num_tasks*ilog2(num_tasks) + 1)) {
		return 1;
	}

	return 0;
}
	


