@Matthew Tay mhtay
@Deeptaanshu Kumar deeptaan
@Kevin Brennan kbrennan

	.global setUserConditions
       	.global globArray
                  
setUserConditions:
	ldr	r4, =linkR
	str	lr, [r4]		@ save kernel stack pointer
	mrs	r4, cpsr		@ load r4 with cpsr
	bic 	r4, r4, #0xff		@ clear all masks
	orr	r4, r4, #0x10		@ set USER mode bits
	msr	cpsr, r4		@ switch to USER mode
	mov 	sp, r0			@ store the latest user stack value
	mov	sp, #0xa3000000		@ set stack pointer to new user stack
	subs	r4, r0, #1		
	blt	end

loop:
	ldr	r5, [r1,r4,asl #2]	@ lsl by 2 to multiply each offset by 4
	stmfd 	sp!, {r5}		@ place argc and argv on user stack
	subs	r4, r4, #0x1
	bpl 	loop	

end:
	stmfd	sp!, {r0}	
	mov	pc, r2			@ load user program 

