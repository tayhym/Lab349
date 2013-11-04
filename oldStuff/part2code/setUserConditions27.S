.global setUserConditions
.global globArray
                             
setUserConditions:
	
	stmfd	sp!, {r1-r12,lr}
	ldr 	r4 ,=globArray
	str 	sp, [r4]

setup:	
	mrs	r4, cpsr		@ load r4 with cpsr
	bic 	r4, #0x1f		@ clear mode bits (last 5 bits)
	orr	r4, r4, #0x0d0		@ mask IRQ and FIQ and set to user mode
	msr	cpsr, r4		@ load new cpsr with masks
	ldr	sp ,=0xa3000000		@ set stack pointer to new user stack

	mov 	r4, r1			@ get (pointer) to argv from crt0.S setup
	mov 	r5, r0			@ get argc from parameter crt0.S setup
	add 	r4, r5, #1		@ get null element addr of argv in svc stack
	stmfd	sp!, {r4}
 	subs 	r4, r4, #1		@ check that num args is not 0
	ble	done

loop: 					
	stmfd 	sp!, {r4}		@ place argc and argv values on user stack
	subs 	r4, r4, #1		@ decrement counter	
	bgt	loop			@ contnue storing elements of argv unto user stack

done:	
	stmfd	sp!, {r0}		@ store arc on stack as last item
	ldr	pc, =0xa2000000		@ set program counter to 0xa2000000
end:	



