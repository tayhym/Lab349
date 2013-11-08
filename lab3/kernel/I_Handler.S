@ Matthew Tay mhtay
@ Deeptaanshu Kumer deeptaan
@ Kevin Brennan kbrennan

@ called when there is an IRQ that updates the main time variable. 
@ 
                             
	.global I_Handler

I_Handler:                                                                     
        ldr sp, =0xa1500000		@ set sp_irq to middle of dram

	sub	sp, sp, #4        	@ leave room on stack for spsr of previous state    
        stmfd	sp!, {r0-r12, lr} 	@ store gp regs, lr to prepare to call swi    
        mrs	r2, spsr            	@ get spsr into gp registers           
        str	r2, [sp, #14*4]     	@ store spsr for return to user mode    

	mrs	r2, cpsr
	orr	r2, r2, #0x08
	msr	cpsr, r2	
		
	bl	C_IRQ_Handler		@ handle interrupt
	ldr	r2, [sp, #14*4]		@ restore spsr
	msr	spsr, r2		@ restore spsr
	ldmfd	sp!, {r0-r12,lr}	@ restore registers from stack
	add	sp, sp, #4		@ reclaim space from spsr
	movs 	pc, lr			@ return to previous mode	

