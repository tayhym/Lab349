@ read.S: read sycall wrapper
@
@ Authors: Kevin Brennan <kbrennan@andrew.cmu.edu>
@          Deeptaanshu Kumar <deeptaan@andrew.cmu.edu>
@	   Matthew Tay <mhtay@andrew.cmu.edu>
@ Date:    2.38am 14/10/2013

@ included for SWI handler address
#include <bits/swi.h>

	.file	"read.S"
	.text

	.global	read

read:
	stmfd sp!, {r4-r12, r14} @ store all callee-saved registers
	SWI READ_SWI	@ no change in params (r0-r2) ARM caller-saved
	ldr r3, =-1 	@ mvn r3, #0 (stores -1 for comparison)
	TST r0, r3 	@ check for negative result (meaning error)
	BPL done
	NEG r0, r0	@ sysCall error, set errno 
  	ldr r3, =errno  @ r3 = address of global variable errno
	str r0, [r3]	@ store to errno 
	mov r0, #-1	@ set wrapper to return -1
			

done: 	@ only valid to test errno when syscall wrapper returns -1
	ldmfd sp!, {r4-r12, r14} @ restore callee-saved registers
	MOV pc, lr


	

	
