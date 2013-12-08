@ Matthew Tay mhtay
@ Deeptaanshu Kumer deeptaan
@ Kevin Brennan kbrennan

@ On entering S_Handler, CPU has switched to SVC mode, and saves cpsr into spsr
@ S_Handler stores registers on stack, stores saved user state (spsr)
@ Before call to exit, restores link register from supervisor stack.
@ This thus supports reentrant calls to S_Handler

#include <arm/psr.h>

.file "S_Handler.S"
        .global S_Handler

S_Handler:
	stmfd 	sp!, {sp, lr}^			@ store context
	sub		sp, sp, #4
	stmfd	sp!, {r0-r12,lr}		@ Store callee registers

	mrs		r2, spsr				@ Store current spsr
	str		r2, [sp, #14*4]
	mov		r1, sp
	ldr		r0,  [lr, #-4]
	bic		r0, r0, #0xff000000		@ Parse swi_num
	
	msr     cpsr_c, #(PSR_MODE_SVC)	@ Switch to supervisor mode

	ldr		r8, =global_data		@ Restore r8
	ldr 	r8, [r8]
	bl		C_SWI_Handler			@ Handle swi call

	msr     cpsr_c, #(PSR_MODE_SVC|PSR_IRQ)	@ Enable interrupts

	ldr		r2, [sp, #14*4]			@ restore spsr
	msr		spsr, r2

	add		sp, sp, #4				@ restore callee registers
	ldmfd	sp!, {r1-r12, lr}
	add		sp, sp, #4
	ldmia	sp!, {sp, lr}^			@ restore context
	movs	pc, lr
