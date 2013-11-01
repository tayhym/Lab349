	.file	"strTableTwo.c"
	.text
	.align	2
	.global	strTableTwo
	.type	strTableTwo, %function
strTableTwo:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r3, #1
	stmfd	sp!, {r4, r5, r6, lr}
	mov	r6, r0
	mov	r4, r3
	mov	r0, r2
	bmi	.L7
	add	r3, r1, r3
	ldr	r5, .L10
	sub	r1, r3, #1
	mov	lr, #0
.L4:
	ldrb	ip, [r1, #0]	@ zero_extendqisi2
	add	lr, lr, #1
	umull	r3, r2, r5, ip
	mov	r2, r2, lsr #4
	mov	r3, r2, asl #5
	sub	r3, r3, r2, asl #3
	rsb	r3, r2, r3
	rsb	r3, r3, ip
	and	r3, r3, #255
	cmp	r3, r0
	strleb	ip, [r3, r6]
	cmp	lr, r4
	sub	r1, r1, #1
	bne	.L4
.L7:
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
.L11:
	.align	2
.L10:
	.word	-1307163959
	.size	strTableTwo, .-strTableTwo
	.ident	"GCC: (GNU) 4.1.2"
