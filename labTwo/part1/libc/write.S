@ write.S: write sycall wrapper
@
@ Authors: Deeptaanshu Kumar <deeptaan@andrew.cmu.edu>
@          Kevin Brennan <kbrennan@andrew.cmu.edu>
@	   Matthew Tay <mhtay@andrew.cmu.edu>
@ Date:    2.38am 14/10/2013

@included for definition of SWI handlers
#include <bits/swi.h>	

	.file	"write.S"
	.text

	.global	write

write:
	@stmfd sp!, {r4-r12,r14} @ store callee saved registers
	SWI WRITE_SWI	@ no change in params (r0-r2) caller-saved in ARM
	ldr r3, =-1	@ load r3 with -1
	TST r0, r3 	@ check for negative result (meaning error)
	BPL done
	NEG r0, r0	@ sysCall error, set errno 
	ldr r3, =errno 	@ loads r3 with address of errno  
  	mov r0, #-1	@ set wrapper to return -1
			

done: 	@ only valid to test errno when syscall wrapper returns -1
	@ldmfd sp!, {r4-r12, r14} @ restore callee saved registers
	MOVS pc, lr
	




