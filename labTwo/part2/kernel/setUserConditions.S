.global setUserConditions
.global oldsp                  
           
setUserConditions:
	sub sp, sp, #-4
	stmfd sp!, {r4-r11,lr}
	mrs r2, spsr
	str r2, [sp, #9*4]
	mov r2, sp
	ldr r3, =oldsp 	
	str r2, [r3]
	
	mrs r2, cpsr 
	orr r2, r2, #0x0d0
	msr cpsr, r2
	ldr sp ,=0xa3000000
	cmp r0, #1
	blt done 

	mov r4, r0
	mov r5, r1 
	
loop: 	
	ldr r6, [r5,r4] 
	stmfd sp!, {r6}
	subs r4, #1 
	bge loop 

done:
	stmfd sp!, {r0}
	ldr pc, =0xa2000000

