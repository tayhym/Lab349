/** @file time.S
 *
 * @brief time sycall wrapper
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

	.file "time.S"
	.global time
time:
	stmfd sp!, {r1-r12,r14}  @ store link to user code 
	SWI TIME_SWI
	ldmfd sp!, {r1-r12,r14}
	mov pc, lr

