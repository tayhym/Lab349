@ exit.S: exit sycall wrapper
@
@ Authors: Matthew Tay <mhtay@andrew.cmu.edu>
@          Kevin Brennan <kbrennan@andrew.cmu.edu>
@	   Deeptaanshu Kumar <deeptaan@andrew.cmu.edu>
@ Date:    2.38am 14/10/2013

#include <bits/swi.h>	

	.file	"exit.S"
	.text

	.global	exit
exit:
	@ added code starting below
	@ OABI convention has r0 as param 1
	SWI EXIT_SWI
