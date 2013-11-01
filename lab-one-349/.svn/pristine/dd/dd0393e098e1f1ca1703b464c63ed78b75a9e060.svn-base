@ exit.S: exit sycall wrapper
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date

#include <bits/swi.h>		@ added for cpp

	.file	"exit.S"
	.text

	.global	exit
exit:
	@ added code starting below
	@ OABI convention has r0 as param 1
	SWI EXIT_SWI
	
	

	
