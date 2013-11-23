/** write.S: write sycall wrapper
 *
 * Authors: Deeptaanshu Kumar <deeptaan@andrew.cmu.edu>
 *          Kevin Brennan <kbrennan@andrew.cmu.edu>
 *	    Matthew Tay <mhtay@andrew.cmu.edu>
 * Date:    19:43 2013-11-17 
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_create.S"
	.text

FUNC(mutex_create)
	stmfd	sp!, {r4-r12,r14} @ store registers
	SWI	 	MUTEX_CREATE	
	ldr 	r3, =-1		@ load r3 with -1
	TST 	r0, r3 		@ check for negative result (meaning error)
	BPL 	done
	NEG 	r0, r0		@ sysCall error, set errno 
	ldr 	r3, =errno 	@ loads r3 with address of errno  
  	str 	r0, [r3]	@ load errno with negative of sysCall return value
	mov 	r0, #-1		@ set wrapper to return -1
			
done: 					@ only valid to test errno when syscall wrapper returns -1
	ldmfd 	sp!, {r4-r12, r14} @ restore registers
	mov 	pc, lr 		@ return back to where-ever in user code it was 			
