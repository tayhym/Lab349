

.global storeRetToGumAddr
.global retToGumAddr
.global globArray
.global calleeSavedRegs

storeRetToGumAddr:
	ldr r2 ,=calleeSavedRegs	
	stmfd r2, {r4-r11, sp, lr}
	movs pc, lr
			


