@Matthew Tay mhtay
@Deeptaanshu Kumar deeptaan
@Kevin Brennan kbrennan

	.global setUserConditions
       	.global globArray   
	@.global pushArgsUntoUserStack                    
setUserConditions:		@ r0 = argc r1 = argv 
	
	@ kernel/svc mode 	
	stmfd	sp!, {r0-r12,lr}
	ldr	r4, =globArray		@ setUp Kernel Stack 
	str	sp, [r4]		@ save kernel stack pointer 

	@ldr r2, =0xa3000000	
	@bl pushArgsUntoUserStack
	
	mrs	r4, cpsr		@ load r4 with cpsr
	bic 	r4, #0x1f		@ clear mode bits (last 5 bits)
	orr 	r4, r4, #0x10		@ set USER_mode	
	msr	cpsr, r4		
	@mov 	sp, r0			@ store the latest user stack value
	

	@ user mode
	mov	sp, #0xa3000000		@ set stack pointer to new user stack
	subs	r4, r0, #1		
	blt	end

loop:
	ldr	r5, [r1,r4,lsl #2]	@ lsl by 2 to multiply each offset by 4
	stmfd 	sp!, {r5}		@ place argv[i] on user stack
	subs	r4, r4, #0x1
	bpl 	loop	

end:
	stmfd	sp!, {r0}		@ store argc on user stack
	mrs 	r4, cpsr
	bic 	r4, #0xc0		@ clear if bits 110_00000. enable interrupts	
	mov	pc, r2			@ load user program (at address r2)

