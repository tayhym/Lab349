
	.text
	.file "S_Handler.S"
	.global S_Handler

.extern C_SWI_Handler
	
S_Handler:                                                                     
	sub sp, sp, #-4
	stmfd sp!, {r0-r12,lr}
	mrs r3, spsr
	ldr r3, [sp,#14*4]
	mov r1, sp

	ldr r3, [lr, #-4]
	bic r3, #0xff000000
	bic r3, #0x900000
	mov r0, r3
	bl C_SWI_Handler

	add sp, sp, #4*4 
	ldmfd sp!,{r2}
	msr spsr, r2 
	movs pc, lr


