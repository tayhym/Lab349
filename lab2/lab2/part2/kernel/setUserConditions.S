@Matthew Tay mhtay
@Deeptaanshu Kumar deeptaan
@Kevin Brennan kbrennan

	.global setUserConditions
       	.global globArray                      
setUserConditions:
	stmfd	sp!, {r0-r12,lr}
	ldr	r4, =globArray
	str	sp, [r4]
	mrs	r4, cpsr		@ load r4 with cpsr
	bic 	r4, #0x1f		@ clear mode bits (last 5 bits)
	orr	r4, r4, #0x0d0		@ mask IRQ and FIQ and set to user mode
	msr	cpsr, r4		@ load new cpsr with masks
	mov	sp, #0xa3000000		@ set stack pointer to new user stack
	subs	r4, r0, #1
	blt	end

loop:
	ldr	r5, [r1,r4,lsl #2]	@ lsl by 2 to multiply each offset by 4
	stmfd 	sp!, {r5}		@ place argc and argv on user stack
	subs	r4, r4, #0x1
	bpl 	loop	

end:
	stmfd	sp!, {r0}	
	ldr	pc, =0xa2000000		@ set program counter to 0xa2000000

