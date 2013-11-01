.global retToGumAddr


retToGumAddr:
	ldr r3 ,=retAddrToGum
	ldr r4, [r3]
	mov lr, r4 
	movs pc, lr

