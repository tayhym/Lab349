.global b_out 
.global oldsp
.global retAddrToGum

b_out: 
	ldr r2 ,=oldsp
	ldr sp, [r2]
	@ldmfd sp!, {r4-r11,lr} 
	@ldmfd sp!, {r2}
	@msr spsr, r2 
	
	ldr r3 ,=retAddrToGum
	str lr, [r3]

	movs pc, lr

