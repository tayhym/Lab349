.global setUserConditions
                             
setUserConditions:
	ldmfd   sp!, {r0-r12, lr}	
	mrs	r4, cpsr		@ change to cpsr, mask irq, fiq 
	bic 	r4, #0x1f		
	orr	r4, r4, #0x0d0		
	msr	cpsr, r4		
	mov	sp, #0xa3000000		@ set up user stack
	
	mov     r4, r1                  @ store argc, argv on user stack
        mov     r5, r0                   
        add	r4, r4, r5
	add     r4, r4, #1              
        ldr 	r6, [r4,#0]
	stmfd   sp!, {r6}                                                       
        subs    r4, r4, #1                        
        ble     done                                                            
                                                                                
loop:   
	ldr 	r6, [r4, #0]                                                                        
        stmfd   sp!, {r6}               
        subs    r4, r4, #1                                   
        bge     loop                     
                                                                                
done:                                                                           
        stmfd   sp!, {r0}              
	ldr	pc, =0xa2000000		@ jump to user program
