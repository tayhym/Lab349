	@ andrewIDs : kbrennan mhtay deeptaan
	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd 	sp!, {r4}    @ store r4 for use later	
	cmp	r3, #1     @ check if (dstLen >= 1)
	bmi     .L11 	     @ exit function if (dstLen<1)
	sub	r3, r3, #1 @ get destLen -1 index 
.L9:
	ldrb	ip, [r1, r3]	@ zero_extendqisi2. get ip = dst[i] 
	mov 	r4, ip		@ store r4 = dst[i]
.L7:
	sub	ip, ip, #23  @ do ip = dst[i] % 23 assuming dst[i] >= 23  
	cmp	ip, #22      
	bgt	.L7
.L13:
	cmp	ip, r2       @ check if dst[i] >= srcLen  	
	strleb  r4, [r0, ip]   @ otherwise store src[ip] = dst[i] (unmoded) 
.L4:
	subs	r3, r3, #1   @ decrement counter and set zero flag (end condition) 
	bne	.L9
.L11:			       @ restore saved variables and program counter 		
	ldm 	sp!, {r4}
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"

