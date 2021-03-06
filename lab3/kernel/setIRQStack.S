@Matthew Tay mhtay
@Deeptaanshu Kumar deeptaan
@Kevin Brennan kbrennan
	
	.global setIRQStack

setIRQStack:
	stmfd	sp!, {r0,r2,r4}
	mrs	r4, cpsr		@ load r4 with cpsr
	mov	r2, r4			@ store SVC spsr
	bic 	r4, r4, #0xff		@ clear mode bits (last 5 bits)
	orr	r4, r4, #0x12		@ mask IRQ, mask FIQ, and set to IRQ mode
	msr	cpsr, r4		@ change to IRQ mode with masks
	mov	sp, r0			@ set stack pointer to new user stack
	bic	r2, r2, #0xec		@ enable IRQ's for SVC mode
	msr	cpsr, r2		@ change back to SVC mode with no masks
	ldmfd	sp!, {r0,r2,r4} 	@ restore kernel registers
	mov	pc, lr			@ return to kernel main

