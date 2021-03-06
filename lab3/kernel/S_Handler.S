@ Matthew Tay mhtay
@ Deeptaanshu Kumer deeptaan
@ Kevin Brennan kbrennan

@ On entering S_Handler, CPU has switched to SVC mode, and saves cpsr into spsr
@ S_Handler stores registers on stack, stores saved user state (spsr)
@ Before call to exit, restores link register from supervisor stack.           
@ This thus supports reentrant calls to S_Handler                              
	.global S_Handler
	.global linkR

S_Handler:                                                                     
        sub	sp, sp, #4        	@ leave room on stack for spsr of (USER)      
        stmfd	sp!, {r0-r12, lr} 	@ store gp regs, lr to prepare to call swi    
        mrs	r2, spsr            	@ get spsr into gp registers
        str	r2, [sp, #14*4]     	@ store spsr for return to user mode
	mrs	r2, cpsr
	bic	r2, r2, #0xec
	msr	cpsr, r2
        mov	r1, sp			@ pointer to parameters on stack        
        ldr	r0, [lr, #-4]		@ extract swi number                    
	bic	r0, r0, #0xff000000	@ extract 24 bit swi number   
	bic	r0, r0, #0x900000	
	cmp	r0, #0x1		@ swi number of 1 is exit call
	bne	normal

exitCall:
	ldr 	r4, =global_data	@ restore table
	ldr 	r8, [r4] 
	bl 	C_SWI_Handler		@ handle exit to set exit status
	ldr	r2, [sp, #14*4]
	msr	spsr, r2
	ldr 	r4, =linkR		@ get address of linkR
	ldr 	lr, [r4]		@ load sp with stored sp in globArray
	mov 	pc, lr			@ return to kernel.c main

normal:
	ldr 	r4, =global_data	@ restore table
	ldr 	r8, [r4] 
	bl 	C_SWI_Handler
	ldr 	r2, [sp, #14*4]		@ extract back user spsr into r2        
        msr 	spsr, r2			@ restore spsr
	add 	sp, sp, #4			@ adjust by 4 becuase r0 is ret val     
        ldmfd 	sp!, {r1-r12, lr} 	@ restore gp registers, lr      
        add 	sp, sp, #4   		@ reclaim space from spsr
	movs 	pc, lr			@ return to user function


