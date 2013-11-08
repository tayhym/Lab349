tasks: 
wire in the irq handler  (done)
write the irq handler: 
	- determine if timer interrupt has occurred 
	- disable all further irq 
	- act on positive timer interrupt: 
		-> initializes the sp_irq to point to 0xa1500000  (done)
		<sub out to C_irq handler> 			  (done)	
		-> loads the match registers without drift
			stores OSMR0 with correct timer value
		-> 
	- ignore all other timer interrupts

	- allow irqs again.

Tasks that must be done in some part of code: 
-> masks all other types of interrupts
-> creating a timer resolution of 10 ms

