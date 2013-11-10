	
	.global setIRQStack

setIRQStack:
	@ kernel/svc mode	
	stmfd	sp!, {r0-r12,lr}
	mrs	r4, cpsr		@ load r4 with cpsr
	bic 	r4, r4, #0xff		@ clear IFT_5ModeBits
	orr	r4, r4, #0x0d1		@ set to 110_10001. IFt_IRQ
	msr	cpsr, r4		@ load new cpsr with masks
	mov	sp, r0			@ set stack pointer to new user stack
	
	@ irq mode 
	mrs 	r2, cpsr	
	bic	r2, r2, #0x1f		@ clear ModeBits - set to ift before jmp user	
	orr 	r2, r2, #0x13		@ set to SVC 
	msr	cpsr, r2
	
	@ kernel/svc mode 
	ldmfd	sp!, {r0-r12,lr} 	@ restore kernel registers
	subs 	pc ,lr, #4		@ return to instruction that was interrupted.
	

	
