/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 */

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */

FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0	     /* preserve value of r8 as uboot jump table address */
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	ldr 	r3, =global_data  /* restore r8 */
	ldr 	r8, [r3]	
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)

	
	@ save current context	
	stmia r1!, {r4-r11, sp, lr}
	
	@load new context
	ldmia r0!, {r4-r11, sp, lr}
	
	@ check whether original function occured before
	cmp lr, #0
	beq launch_task  @ launch new task
	@ otherwise, jump back to where original function left off
	mov pc, lr



/* r0 points to target context */
/*add your code to perform a half context switch */	
FUNC(ctx_switch_half)
	
	@load new context
	ldmia r0!, {r4-r11, sp, lr}

	@ check whether original function occured before
	cmp lr, #0
	beq launch_task  @ launch new task
	@ otherwise, jump back to where original function left off
	mov pc, lr
	




