@ Matthew Tay mhtay@andrew.cmu.edu
@ Deeptaanshu Kumer deeptaan@andrew.cmu.edu
@ Kevin Brennan kbrennan@andrew.cmu.edu

@ On entering S_Handler, CPU has switched to SVC mode, and saves cpsr into spsr
@ S_Handler stores registers on stack, stores saved user state (spsr)
@ Before call to exit, restores link register from supervisor stack.           
@ This thus supports reentrant calls to S_Handler                              

	.global S_Handler
	.global exit_status
	.global globArray

S_Handler:                                                                     
        sub	sp, sp, #4          	@ leave room on stack for spsr                 
        stmfd	sp!, {r0-r12, lr} 	@ store gp regs, lr to prepare to call swi     
        mrs	r2, spsr            	@ get spsr into gp registers                   
        str	r2, [sp, #14*4]     	@ store spsr for return to user mode    
        mov	r1, sp     	        @ pointer to parameters on stack          
        ldr	r0, [lr, #-4]   	@ extract swi number                      
	bic	r0, r0, #0xff000000	@ extract 24 bit swi number   
	bic	r0, r0, #0x900000	
	cmp	r0, #0x1		@ swi number of 1 is exit call
	bne	normal
exitCall:
	bl 	C_SWI_Handler		@ handle exit to set exit status
	ldr 	r4, =globArray		@ get address of globArray
	ldr 	sp, [r4]		@ load sp with stored sp in globArray
	ldr 	r4, =linkR		@ get return addr to main 
	ldr 	lr, [r4] 
	ldmfd 	sp!, {r0-r12}		@ restore kernel's registers
	add 	sp, sp, #4 		@ new linkR is back to main 	
	movs 	pc, lr			@ return to kernel.c main
normal:
	bl 	C_SWI_Handler
	ldr 	r2, [sp, #14*4] 	@ extract back user spsr into r2                        
        msr 	spsr, r2          	@ restore spsr
	add 	sp, sp, #4		@ adjust by 4 becuase r0 is ret val                                        
        ldmfd 	sp!, {r1-r12, lr} 	@ restore gp registers, lr      
        add 	sp, sp, #4   		@ reclaim space from spsr
	movs 	pc, lr			@ return to user function


