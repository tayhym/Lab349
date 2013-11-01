	@.file "swi_Handler.c"
	@.text 
	@.align 2 
	@.global S_Handler 
	@.type S_Handler, %function

@IMPORT C_SWI_Handler 
@EXPORT S_Handler
PRESERVE8
EXPORT S_Handler 
AREA S_Handler, CODE, READONLY 
IMPORT C_SWI_Handler

@ On entering S_Handler, CPU has switched to SVC mode, and saved cpsr into spsr
@ S_Handler stores registers on stack, stores saved user state (spsr) 
@ This thus supports reentrant calls to S_Handler
S_Handler:			
	sub sp, sp, #4		@ leave room on stack for spsr 
	stmfd sp!, {r0-r12, lr} @ store gp regs, lr to prepare to call swi
	mrs r2, spsr 		@ get spsr into gp registers
	str r2, [sp, #14*4] 	@ store spsr above gp registers and lr 
	mov r1, sp		@ pointer to parameters on stack
	ldr r0, [lr, #-4]	@ extract swi number
	bic r0, #0xff000000	@ extract 24 bit swi number
	bl  C_SWI_Handler	@ call C_SWI_Handler
	ldr r2, [sp, #14*4]	@ restore spsr 
	msr spsr, r2
	ldmfd sp!, {r0-r12, lr} @ restore gp registers, lr 
	add sp, sp, #4
	movs pc, lr		@ return from handler
END
