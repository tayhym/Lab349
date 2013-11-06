/*
 * Matthew Tay mhtay@andrew.cmu.edu	
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 */
#include <bits/swi.h> 
#include <c_functions.h> 
#include <bits/errno.h>
#include <bits/fileno.h>
#include <exports.h>
#include <asm_functions.h>

/* internal function prototypes specific to file */ 
int service_SWI_Exit(unsigned int exit_status);
int service_SWI_Read(unsigned int *regs);
int service_SWI_Write(unsigned int *regs);

/* does the pushing of argv elements unto user stack, prints out for check */
unsigned int *pushArgsUntoUserStack(unsigned int argc, char **argv, unsigned int *stackPointer) {
	int i;
	for (i = 0; i< argc; i++) {
		printf("argc value is %x\n", argc);
		printf("argv value is %s\n",argv[i]);
		stackPointer--;
		stackPointer[i] = argv[i];
	}
	stackPointer--;
	*stackPointer = argc; 
	printf("number of argc last stored on stack: %x at location %x \n",*stackPointer, stackPointer);
	return stackPointer;	
}







