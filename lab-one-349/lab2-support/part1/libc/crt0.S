@ crt0.S: Execution startup routine
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date


@ for definition of SWI handler address
#include <bits/swi.h>	

	.file	"crt0.S"
	.text

	.global	_start
_start:
	@ Needs more code.
	@ Added some code below 

	@ by default, stack is full descending 
	@ get argc from top of stack 

	ldr r0, [sp], #4  @ load from stack, increment after
	
	@ get address of the first argv param 
	@ already arranged neatly in stack memory 
 	
	mov r1, sp
	BL main		@ not sure about this part.. main?
	@ if main calls mov pc, lr (it returns back to this function)
	
	SWI EXIT_SWI	
		
	




	
