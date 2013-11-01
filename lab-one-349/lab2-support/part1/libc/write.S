@ write.S: write sycall wrapper
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date
@ not sure if http://www.coranac.com/tonc/text/asm.htm is useful.

@included for definition of errno and SWI handlers
#include <bits/swi.h>	

	.file	"write.S"
	.text

	.global	write

write:
	@ Needs more code.
	@ Added some code below
	

	SWI WRITE_SWI	@ no change in params (r0-r2)
	ldr r3, =-1	@ load r3 with -1
	TST r0, r3 	@ check for negative result (meaning error)
	BPL done
	NEG r0, r0	@ sysCall error, set errno 
	ldr r3, =errno 	@ loads r3 with address of errno  
  	mov r0, #-1	@ set wrapper to return -1
			

done: 	@ only valid to test errno when syscall wrapper returns -1
	MOV pc, lr


@errorNum: .word errno  



