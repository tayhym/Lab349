/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 * Revised by: 
 * @Matthew Tay mhtay
 * @Deeptaanshu Kumar deeptaan
 * @Kevin Brennan kbrennan
 */

#include <asm.h>
#include <bits/swi.h>

	.file "sleep.S"

FUNC(sleep)
	stmfd sp!, {r0-r12,r14}  @ store link to user code 
	SWI SLEEP_SWI
	ldmfd sp!, {r0-r12,r14}
	mov pc, lr
