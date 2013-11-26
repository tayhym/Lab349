@Matthew Tay mhtay
@Deeptaanshu Kumar deeptaan
@Kevin Brennan kbrennan

	.global setUserConditions
       	.global globArray
                  
setUserConditions:
	stmfd	sp!, {lr}
	mrs		r4, cpsr	@ load r4 with cpsr
	bic 	r4, r4, #0xff	@ clear all masks
	orr		r4, r4, #0x10	@ set USER mode bits
	msr		cpsr, r4	@ switch to USER mode
	mov		sp, r0		@ set stack pointer to new user stack
	mov		pc, r1		@ load user program 

