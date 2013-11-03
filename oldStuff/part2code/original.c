/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Group Member 1 <email address>
 *         Group Member 2 <email address>
 * Date:   The current time & date
 */

/* swi handler written in c that is called by assembly
   swi handler */

#include <c_functions.h>
#include <exports.h>
#include <bits/swi.h>
#include <asm_functions.h>

extern int S_Handler(void);
extern void setUserConditions( void );

int main(int argc, char *argv[]) {
	puts("a_being called\n");
	unsigned  offset, isNeg, SWI_Handler_Addr;

	/* check if instruction is ldr pc, [pc, #imm12]
	   instruction is assumed to be at 0x08 in vector table */
	if (((*((unsigned int *)0x08)) ^ 0xe59FF000 ) >> 12 ) {
		return 0xbadc0de;
	}
	/* check if offset is positive or negative */
	offset = (*((unsigned int *)0x08)) & 0x0FFF;
	isNeg = (offset & 0x0800) >> 11;
	
	/* find address of U-Boot's SWI Handler */
	/* addr  = 0x08 + preFetchOffset + offset */
	if (offset >> 11) {
		puts("in if\n");
		offset = ( ~offset + 1 ) & 0x7FF;
		SWI_Handler_Addr = *((unsigned int *)( 0x08 + 0x8  - offset));
	}
	else {
		puts("in else\n");
		SWI_Handler_Addr = *((unsigned int *)( 0x08  + 0x8 + offset));
	}
	puts("b_being called \n");
	//putc((char)(offset+65));
	//SWI_Handler_Addr = (unsigned) S_Handler; 
	SWI_Handler_Addr = *((unsigned int *)0x24);
	putc((char)((SWI_Handler_Addr  & 0xf)+48));
	putc((char)(((SWI_Handler_Addr >> 4) & 0xf) + 48));
	putc((char)(((SWI_Handler_Addr >> 8) & 0xf) + 48));
	putc((char)(((SWI_Handler_Addr >> 12) & 0xf) + 48));
	putc((char)(((SWI_Handler_Addr >> 16) & 0xf) + 48));
	putc((char)(((SWI_Handler_Addr >> 20) & 0xf) + 48));
	putc((char)(((SWI_Handler_Addr >> 24) & 0xf) + 48));
	putc((char)(((SWI_Handler_Addr >> 28) & 0xf) + 48));
	
	unsigned int old_vector = install_Handler((unsigned int)&S_Handler,(unsigned int *) 0x5c0009c0);
	//unsigned int old_vector = install_Handler((unsigned int ) SWI_Handler_Addr, (unsigned int *)&S_Handler);
	//putc(offset);
	/* Switch to user moder with IRQ's & FIQ's masked */
	setUserConditions( );
	puts("c\n");
	
 	/* save old_vector to restore when kernel exits */
 	puts("d\n");	
	/* restore U-Boot's SWI Handler */
	*((unsigned int *)SWI_Handler_Addr) = old_vector;

	return -255;
}




/* installs swi handler by clobbering first 2 bytes of
   u-boot's swi handler (located at swi addr 0x24) */

/* does: clobbers 8 bytes of memory at location to point to new SWI 
 *  	 handler (puts a ldr pc, [pc, #imm12bit]) such that pc executes
 * 	 at [vector].
 * returns: contents of old swi handler 
 */
unsigned int install_Handler(unsigned int location, unsigned int *vector) {
	unsigned offset;
	unsigned vec, oldVecContents;
	/* determine amount pc must jump by after prefetch offset */
	offset = (location - (unsigned) vector - 0x8);
	putc((char) ((offset>>12)+65));
	if (offset & 0xFFFFF000) {
		/* print error message if swi_handler's address can't be
		   reached with 12 bits (4KB) */
 		printf("Installation of handler failed (out of range)");
		return 0xbadC0de;
	}
	
	/* ldr pc, [pc, #imm12bit] instruction has opcode 0xe59ff000*/
	vec = (offset | 0xe59ff000);
 	/* save old instructions located at vector */
	oldVecContents = *vector;
	/* replace contents of uboot's swi handler with jump instruction */
	*vector = vec;
	return oldVecContents;
}

	



	

 		  
 


