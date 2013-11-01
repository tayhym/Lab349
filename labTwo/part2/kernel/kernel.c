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
extern void setUserConditions(void);

/* set up global integer array for error number and for return status to uBoot
* globArray[0] = link register 
* globArray[1] = error status
*/
int errNum; 
int oldsp;
int retAddrToGum; 

int main(int argc, char *argv[]) {
	asm volatile (                                                        
                "ldr r3, =retAddrToGum\n\t"                                   
                "str lr, [r3]\n\t" 
		"ldr r3, =retAddrToGum\n\t"
		"str sp, [r3]\n\t"              
                ::: "r3", "cc", "memory"                                      
        ); 
	
	printf("KERNEL.C \n\n");
	unsigned  int offset;
	unsigned int *uBootSwiAddr;
	unsigned int instruc; 
	unsigned int vecHoldingUBootSwiAddr;
	unsigned int isNeg=0;
	unsigned int *swiHandlerAddr;
	unsigned int oldInstrucOne; 
	unsigned int oldInstrucTwo; 
	unsigned int *addrHoldingSwiHandlerAddr; 
	
        printf("value of lr to gum: %x\n", retAddrToGum); 
	
	/* check instruction at 0x8 is ldr pc, [pc, #imm12] */
	instruc = *((unsigned int *)0x8);
	if ((instruc ^ 0xe59FF000) >> 12) {
		return 0xbadc0de;
	}
	/* determine offset from pc, set isNeg if offset negative  */
	offset = (*((unsigned *)0x08)) & 0x0FFF;
	if (instruc & 0x800) {
		isNeg = 1;
	 	// calculate the absolute value of offset
		offset = ~offset + 1;	
	}  
	/* calculate address holding uBoot_SWI_addr 
	 * determined by adding signed offset to pc, including prefetch
	 */
	if (~isNeg) {
		vecHoldingUBootSwiAddr = 0x8 + (int) offset + 0x8; 
	}
	else {
		
		vecHoldingUBootSwiAddr = 0x8 - (int)offset + 0x8;
	}
	uBootSwiAddr =(unsigned int *) *((unsigned int*)vecHoldingUBootSwiAddr);	
	/* install into uBootSwiAddr 2 instructions 
	 * 	first instruction is ldr pc, [pc, #imm12]
	 * 	second instruction is address holding address of (own)swiHandler
	 * takes 2 params
	 *	first param is the swiHandlerAddr 
	 *	second param is uBootSwiAddr
	 */
	addrHoldingSwiHandlerAddr = ((unsigned int *)uBootSwiAddr + 1); 
	
	/* save old instructions at uBootSwiAddr */
	oldInstrucOne = *uBootSwiAddr;
	oldInstrucTwo = *((unsigned int *)uBootSwiAddr + 1); 
	
	/* store into addrHoldingSwiHandlerAddr swiHandlerAddr */
	swiHandlerAddr = (unsigned int *)S_Handler;
	*addrHoldingSwiHandlerAddr = (unsigned) swiHandlerAddr; 
	printf("installing swi handler...");
	install_Handler((unsigned int)addrHoldingSwiHandlerAddr, uBootSwiAddr);
	printf("installed\n");
 	
	printf("setting user condition...");
	setUserConditions();
	printf("set\n");
	*uBootSwiAddr = oldInstrucOne;
	*((unsigned int *)uBootSwiAddr + 1) = oldInstrucTwo;
	retAddrToGum = (int) *((unsigned int *)uBootSwiAddr + 2);	
	return errNum;
}





/* installs swi handler by clobbering first 2 bytes of
   u-boot's swi handler (located at swi addr 0x24) */

/* does: clobbers 8 bytes of memory at location to point to new SWI 
 *  	 handler (puts a ldr pc, [pc, #imm12bit]) such that pc executes
 * 	 at [vector].
 */
void install_Handler(unsigned int swiHandlerAddr, unsigned int *uBootSwiAddr) {
	int offset;
	int isNeg = 0;
	unsigned int ldrOpcode;
	/* determine amount pc must jump by after prefetch offset */
	offset = ((int)swiHandlerAddr - ((int) uBootSwiAddr + 0x8));
	
	/* determine if offset is negative */
	if (offset < 0) {
		isNeg = 1;
		// caculate absolute value 
		offset = ~offset + 1; 
	}
	/* if negative offset, set up/down opcode bit 24 to 0 */
	if (isNeg) {
		ldrOpcode = 0xe51ff000 | ((unsigned int) offset);
	}
	/* if positive offset, set up/down opcode bit 24 to 1 */
	else { 
		ldrOpcode = 0xe59ff000 | ((unsigned int) offset);
	}
	 		
	if (offset & 0xFFFFF000) {
		/* print error message if swi_handler's address can't be
		   reached with 12 bits (4KB) */
 		printf("Installation of swiHandler failed - out of range.");
	}
	/* store ldr pc, [pc, #imm12bit] opcode into uBootSwiAddr */
	
	*uBootSwiAddr = ldrOpcode; 
}

	



	

 		  
 


