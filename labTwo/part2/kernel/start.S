@ start.S: Kernel startup routine
@
@ Author: Mike Kasick <mkasick@andrew.cmu.edu>
@ Date:   Tue, 23 Oct 2007 11:20:33 -0400

	.file	"start.S"
	.text

	.global	_start
	.global retAddrToGum
_start:
	@ Branch immediately to main without altering the execution environment.
	@ This indirection is needed to guarantee that the program entry point is
	@ LOAD_ADDR (probably 0xa3000000) without placing strict
	@ requirements on the layout of kernel.c.
@	mov	r3, pc
@	add	r3, r3, #0x4
	
	@ldr r3 ,=retAddrToGum 
	@str lr, [r3]
	
	b	main
