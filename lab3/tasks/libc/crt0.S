@ crt0.S: Execution startup routine
@
@ Authors: Matthew Tay <mhtay@andrew.cmu.edu>
@          Kevin Brennan <kbrennan@andrew.cmu.edu>
@    	   Deeptaanshu Kumar <deeptaan@andrew.cmu.edu>		
@ Date:    2.38am 14/10/2013


@ for definition of SWI handler address
#include <bits/swi.h>	

	.file	"crt0.S"
	.text

	.global	_start
_start:
	
	@ by default, stack is full descending 
	@ get argc from top of stack 

	ldr r0, [sp], #4  @ load from stack, increment after
	
	@ get address of the first argv param 
	@ already arranged neatly in stack memory 
 	
	mov r1, sp
	stmfd sp!, {r14}@ preserve link register overwritten during branch
	BL main		@ branches to main function
	@ if main calls mov pc, lr (it returns back to this function)
	ldmfd sp!, {r14} @ restore link register used during branch 	
	SWI EXIT_SWI	
		
	




	
