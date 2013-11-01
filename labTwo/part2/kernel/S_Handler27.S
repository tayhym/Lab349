@#include <c_functions.h>
@extern void C_SWI_Handler(unsigned int swi_num, unsigned int *regs);

@.extern C_SWI_Handler

@ CPU in SVC mode, and saves cpsr into spsr
@ S_Handler stores registers on stack, stores saved user state (spsr)
@ Before call to exit, restores link register from supervisor stack.           
@ This thus supports reentrant calls to S_Handler                              

.global globArray
.global S_Handler
S_Handler:                                                                     
        stmfd sp!, {lr}
	sub sp, sp, #4          @ store argument registers unto sup stack, 
			 	@ giving C_Swi_Handler swiNum and pointer
 		                
        stmfd sp!, {r0-r12, lr} @ store gp regs, lr to prepare to call swi     
        mrs r2, spsr            @ get spsr into gp registers                   
        str r2, [sp, #14*4]     @ store spsr for return to user mode    
        mov r1  ,sp  	        @ pointer to parameters on stack          
        ldr r4, [lr, #-4]       @ extract swi number                      
	bic r4, r4, #0xff000000	@ extract 24 bit swi number   
	bic r4, r4, #0x900000 	@ mask out 0x900000 of 24 bit swi number for easy computation
	cmp r4, #1		@ swi number of 1 is exit call
	bne normal
	
exitCall:
	bl C_SWI_Handler
	add sp, sp, #4
	ldmfd sp!, {r1-r12,lr}	@ restore context of registers
	add sp, sp, #4		@ not necessary to save user cpsr (in spsr) when exit 		
	ldmfd sp!, {lr}			
	movs pc, lr		 
 		
normal:
	bl  C_SWI_Handler       @ call C_SWI_Handler                      
	ldr r2, [sp, #14*4]     @ extract back user spsr into r2                        
        msr spsr, r2          	@ restore spsr                                        
        ldmfd sp!, {r0-r12, lr} @ restore gp registers, lr      
        add sp, sp, #4   
	movs pc, lr		@ return from handler


