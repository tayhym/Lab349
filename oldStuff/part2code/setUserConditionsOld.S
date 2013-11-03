	.global setUserConditions
                             
setUserConditions:                                                    
	mrs	r4, cpsr		@ load r4 with cpsr
	bic 	r4, #0x1f		@ clear mode bits (last 5 bits)
	orr	r4, r4, #0x0d0		@ mask IRQ and FIQ and set to user mode
	msr	cpsr, r4		@ load new cpsr with masks
	ldr	sp, =0xa3000000
	subs 	r2, r0, #1
	blt	end
loop:
	ldr	r4, [ r1, r2 ]
	stmfd   sp!, {r4}
	subs	r2, r2, #1
	blt	loop
	
end:	
	stmfd 	sp!, {r0}		@ place argc on stack even if 0 arguments
	ldr	pc, =0xa2000000
	@movs 	pc, lr			@ return to kernel.c

