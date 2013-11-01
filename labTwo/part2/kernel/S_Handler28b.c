
@ S_Handler: 

.global S_Handler
.global retAddrToGum
S_Handler:                                                                     
	
	sub sp, sp, #4          @ store argument registers unto sup stack, 
			 	@ giving C_Swi_Handler swiNum and pointer
 		                
        stmfd sp!, {r0-r12, lr} @ store gp regs, lr back to S_Handler    
        mrs r2, spsr            @ get spsr into gp registers                   
        str r2, [sp, #14*4]     @ store spsr for return to user mode    
        mov r1  ,sp  	        @ pointer to parameters on stack          
        ldr r0, [lr, #-4]       @ extract swi number                      
	bic r0, r0, #0xff000000	@ extract 24 bit swi number   
	bic r0, r0, #0x900000 	@ mask out 0x900000 of 24 bit swi number for easy computation
	
@	cmp r0, #1;

@	beq exit

	bl C_SWI_Handler
	ldr r2, [sp, #14*4]
	msr spsr, r2 
	ldmfd sp!, {r0-r12,lr}	@ restore context of registers
	add sp, sp, #4		@ not necessary to save user cpsr (in spsr) when exit 		
	movs pc, lr		 
 
@exit:
@	bl C_SWI_Handler
@	ldr r2, [sp, #14*4]
@	msr spsr, r2
@	ldmfd sp!, {r0-r12, lr}
@	add sp, sp, #4
@	ldr r3, =retAddrToGum 
@	ldr lr, [r3] 
@	movs pc, lr

		
