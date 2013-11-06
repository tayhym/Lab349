#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#define VTABLE_ADDR 0x08
#define USER_ADDR 0xa0000000

uint32_t global_data;
extern int S_Handler(void);
extern int setUserConditions(int argc, char *argv[], int addr);

volatile int globArray[1];
volatile int linkR[1];

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	asm volatile("stmfd sp!, {r4-r11,lr}");
	asm volatile("ldr r4, =linkR");
	asm volatile("str sp, [r4]");

        unsigned int offset;                                                 
        unsigned int *uBootSwiAddr;                                           
        unsigned int instruc;                                                 
        unsigned int oldInstrucOne;                                           
        unsigned int oldInstrucTwo;  

	/* check instruction at 0x8 is ldr pc, [pc, #imm12] */                
        instruc = *((unsigned *)VTABLE_ADDR);                     
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
                uBootSwiAddr = (unsigned *)*((unsigned *)(VTABLE_ADDR+(int)offset+0x8));             
        }
                                                                                
        /* save old instructions at uBootSwiAddr */                             
        oldInstrucOne = *uBootSwiAddr;                                          
	oldInstrucTwo = *((unsigned *)uBootSwiAddr + 1);                    
 
        /* store new ldr instruction and swi handler addr in uboot swi handler */               
        *((unsigned *)uBootSwiAddr + 1 ) = (unsigned)((unsigned *)S_Handler);                 
	*uBootSwiAddr = 0xe51ff004;

	/* Set up user space and jump to user function */
	int status = setUserConditions(argc,argv,USER_ADDR);	

	/* Restore U-Boot's SWI Handler */
	*uBootSwiAddr = oldInstrucOne;                                          
        *((unsigned *)uBootSwiAddr + 1) = oldInstrucTwo;                    
	
	asm volatile("ldr r4, =linkR");
	asm volatile("ldr sp, [r4]");
	asm volatile("ldmfd sp!, {r4-r11,lr}");

	return status;
}
