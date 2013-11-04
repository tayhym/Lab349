 .text
        .file "S_Handler.S"
        .global S_Handler

.extern C_SWI_Handler
        
S_Handler:
        sub sp, sp, #-4
        stmfd sp!, {r0-r12,lr}
        mrs r3, spsr
        str r3, [sp,#14*4]
        mov r1, sp

        ldr r3, [lr, #-4]
        bic r3, #0xff000000
        mov r0, r3
        bl C_SWI_Handler

        ldr r3, [sp,#14*4]
	msr spsr, r3
	ldmfd sp!, {r0-r12,lr}
        add sp, sp, #4
        movs pc, lr
