	
	.global setIRQStack

setIRQStack:
	@mrs	r4, cpsr		@ load r4 with cpsr
	@mov	r2, r4
	@bic 	r4, #0x1f		@ clear mode bits (last 5 bits)
	@orr	r4, r4, #0x0d1		@ mask IRQ, mask FIQ, and set to IRQ mode 1101_0001
	@msr	cpsr, r4		@ load new cpsr with masks
	
	mov 	r3, #0xd1		
	msr 	cpsr, r3		@ 110_10001 IFt_IRQ	
	mov	sp, r0			@ set stack pointer to new user stack
	
		
	@bic	r2, r2, #0xec		@ bit clears 111_01100
	@msr	cpsr, r2
	
	mov 	r3, #0xd3		@ 110_10011 IFt_SVC
	msr 	cpsr, r3	
	mov	pc, lr			@ return to kernel main

	
