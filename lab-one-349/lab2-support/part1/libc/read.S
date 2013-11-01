@ read.S: read sycall wrapper
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date
@ referenced: http://lists.busybox.net/pipermail/uclibc/2005-June/011894.html

@ included for definition of extern errno and for SWI handler address
#include <bits/swi.h>
@#include <errno.h>	

	.file	"read.S"
	.text

	.global	read
@	IMPORT errno


read:
	@ Needs more code.
	@ Added some code below
	
	SWI READ_SWI	@ no change in params (r0-r2)
	ldr r3, =-1 	@ mvn r3, #0 (stores -1 for comparison)
	TST r0, r3 	@ check for negative result (meaning error)
	BPL done
	NEG r0, r0	@ sysCall error, set errno 
	@str r0, [errorNum, #0]
  	ldr r3, =errno  @ r3 = address of global variable errno
	str r0, [r3]	@ store to errno 
	mov r0, #-1	@ set wrapper to return -1
			

done: 	@ only valid to test errno when syscall wrapper returns -1
	MOV pc, lr


@errorNum: .word errno  
	

	
