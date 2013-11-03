/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Matthew Tay <mhtay@andrew.cmu.edu>
 *         Deeptaanshu Kumar <deeptaan@andrew.cmu.edu>
	   Kevin Brennan <kbrennan@andrew.cmu.edu>
 * Date:   11:46 10/27/2013
 */

/* swi handler written in c that is called by assembly
   swi handler */

#include <c_functions.h>
#include <exports.h>
#include <bits/swi.h>
#include <asm_functions.h>

extern int S_Handler(void);
extern void setUserConditions(void);

/* set up global integers for error number and for return status to main
* globArray[0] = link register
*/
int linkR;  
int exit_status;
int globArray;
                                                                            
int main(int argc, char *argv[]) {                                            

        unsigned int offset;                                                 
        unsigned int *uBootSwiAddr;                                           
        unsigned int instruc;                                                 
        unsigned int oldInstrucOne;                                           
        unsigned int oldInstrucTwo;                                           
                                                                       
        /* check instruction at 0x8 is ldr pc, [pc, #imm12] */                
        instruc = *((unsigned int *)0x8);                     
        if ((instruc ^ 0xe59ff000) >> 12) {                                   
                return 0xbadc0de;                             
        }                        
                             
        /* parse offset from last 12 bits of instruc  */
  	offset = instruc & 0x0fff; 
                             
        /* calculate address holding uBoot_SWI_addr                           
         * determined by adding signed offset to pc, including prefetch       
         */                                                                   
        if (offset>>11) {                                                         
        	return 0xbadc0de;
	}                                                                     
        else {                                                                
                uBootSwiAddr = (unsigned int *)*((unsigned int*)( 0x8 + (int)offset + 0x8));             
        }
                                                                                
        /* save old instructions at uBootSwiAddr */                             
        oldInstrucOne = *uBootSwiAddr;                                          
	oldInstrucTwo = *((unsigned int *)uBootSwiAddr + 1);                    
 
        /* store new ldr instruction and swi handler addr 
	in uboot swi handler */               
        *((unsigned int *)uBootSwiAddr + 1 ) = 
		(unsigned)((unsigned int *)S_Handler);                 
	*uBootSwiAddr = 0xe51ff004;

	printf("calling user function...");
	/* Set up user space, store return address(main) 
	and jump to user function */
	setUserConditions();  
	printf("returned \n");
	/* Restore U-Boot's SWI Handler */
	*uBootSwiAddr = oldInstrucOne;                                          
        *((unsigned int *)uBootSwiAddr + 1) = oldInstrucTwo;                    
	
	return exit_status;
}

