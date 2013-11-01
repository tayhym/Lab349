#include "c_functions.h"

/* installs swi handler by clobbering first 2 bytes of                          
   u-boot's swi handler (located at swi addr 0x24) */                           
                                                                                
/* does: clobbers 8 bytes of memory at location to point to new SWI             
 *       handler (puts a ldr pc, [pc, #imm12bit]) such that pc executes         
 *       at [vector].                                                           
 * returns: contents of old swi handler                                         
 */                                                                             
unsigned int install_Handler(unsigned int location, unsigned int *vector) {     
        unsigned offset;                                                        
        unsigned vec, oldVecContents;                                           
        /* determine amount pc must jump by after prefetch offset */            
        offset = (location - (unsigned) vector - 0x8);                          
                                                                                
        if (offset & 0xFFFFF000) {                                              
                /* print error message if swi_handler's address can't be        
                   reached with 12 bits (4KB) */                                
                printf("Installation of handler failed (out of range)");        
                exit(0);                                                        
        }
	/* ldr pc, [pc, #imm12bit] instruction has opcode 0xe59ff000*/     
        vec = (offset | 0xe59ff000);                                       
        /* save old instructions located at vector */                      
        oldVecContents = *vector;                                          
        /* replace contents of uboot's swi handler with jump instruction */
        *vector = vec;                                                     
        return oldVecContents;                                             
}  






     
