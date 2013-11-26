#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <assert.h>
#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <config.h>

#define SWI_ADDR 0x08
#define IRQ_ADDR 0x18
#define IRQ_SP 0xa1500000
#define LDR_PC 0xe59ff000
#define LDR_INSTR 0xe51ff004

extern int S_Handler(void);
extern void setIRQStack(unsigned int sp);
extern int setUserConditions(unsigned int sp, int addr);

uint32_t global_data;
volatile uint32_t clock;
unsigned int resolution = OS_TICKS_PER_SEC;

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup(); /* bss is valid after this point */
	global_data = table;

        unsigned int instr;
	unsigned int offset;                                                 
        unsigned int *kernelSwiAddr;
	unsigned int *kernelIrqAddr;                                             
        unsigned int swiInstrOne;                                           
        unsigned int swiInstrTwo;
	unsigned int irqInstrOne;                                           
        unsigned int irqInstrTwo;

	/* check instruction at 0x08 is ldr pc, [pc, #imm12] */
	/* check if imm12 is positive */                
        instr = *((unsigned int*)SWI_ADDR);
	offset = (int)(instr & 0x0fff);                  
        if (((instr ^ LDR_PC) >> 12) || (offset >> 11)) {                                 
                return 0xbadc0de;                             
        }                                                                      
        else {                                                                
                kernelSwiAddr = (unsigned int*)*((unsigned int*)(SWI_ADDR + offset + 0x8));             
        }

	/* check instruction at 0x18 is ldr pc, [pc, #imm12] */
	/* check if immediate offset is positive */                 
        instr = *((unsigned int*)IRQ_ADDR); 
	offset = (int)(instr & 0x0fff);                  
        if (((instr ^ LDR_PC) >> 12) || (offset >> 11)) {            
                return 0xbadc0de;                             
        }                                                                    
        else {                                                                
                kernelIrqAddr = (unsigned int*)*((unsigned int*)(IRQ_ADDR + offset + 0x8));             
        }
                                                                                
        /* Save old SWI and IRQ Handler Instructions */                             
        swiInstrOne = *kernelSwiAddr;                                          
	swiInstrTwo = *((unsigned int*)kernelSwiAddr + 1);
	irqInstrOne = *kernelIrqAddr;                                          
	irqInstrTwo = *((unsigned int*)kernelIrqAddr + 1);                   
 
        /* Store load and jump address instructions */
	*kernelSwiAddr = LDR_INSTR;        
	*((unsigned int*)kernelSwiAddr + 1 ) = (unsigned int)((unsigned int*)S_Handler);
	*kernelIrqAddr = LDR_INSTR;        
	*((unsigned int*)kernelIrqAddr + 1) = (unsigned int)((unsigned int*)irq_wrapper);                  

	setIRQStack(IRQ_SP);	
	
	/* Initialize timer, not using IRQ stack-> uses SVC stack */
	clock = 0;

	/* Calculate time between IRQs based on desired resolution */
	offset = (resolution * OSTMR_FREQ_VERDEX)/1000;

	/* Setup memory mapped registers for timer*/
	reg_write(OSTMR_OSMR_ADDR(0), offset);	      // Set match register to desired offset
	reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);      // Set OSMR0 match register to active
	reg_clear(INT_ICLR_ADDR, (1<<INT_OSTMR_0));   // Ensure that timer interrupt is always IRQ 
	reg_set(INT_ICMR_ADDR, (1<<INT_OSTMR_0));     // reg_set(INT_ICMR_ADDR, INT_OSTMR_0);
	reg_write(OSTMR_OSCR_ADDR, 0x0); 	      // Reset counter

	/* Set up user space and jump to user function */
	setUserConditions(USR_STACK, USR_START_ADDR);	
		
	assert(0);        /* should never get here */
}
