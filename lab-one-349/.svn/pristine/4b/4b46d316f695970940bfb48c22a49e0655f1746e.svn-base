	.file	"hello.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hello world!\012\000GoodBye World!\012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	ldr	r3, .L3
	ldr	r3, [r3, #0]
	ldr	r0, .L3+4
	mov	r1, #1
	mov	r2, #29
	bl	fwrite
	mov	r3, #42
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, lr}
	bx	lr
.L4:
	.align	2
.L3:
	.word	stdout
	.word	.LC0
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2"
