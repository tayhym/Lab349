#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>

#define SWI_ADDR 0x08
#define IRQ_ADDR 0x18
#define USER_ADDR 0xa0000000
#define IRQ_SP 0xa1000000

#define LDR_PC 0xe59ff000
#define LDR_INSTR 0xe51ff004

#define OFFSET 36864

extern int S_Handler(void);
extern int I_Handler(void);
extern void setIRQStack(unsigned int sp);
extern int setUserConditions(int argc, char *argv[], int addr);

uint32_t global_data;
volatile uint32_t clock;

volatile int globArray[1];
volatile int linkR[1];

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	asm volatile("stmfd sp!, {r4-r11,lr}");
	asm volatile("ldr r4, =linkR");
	asm volatile("str sp, [r4]");

        unsigned int instr;
	unsigned int offset;                                                 
        unsigned int *kernelSwiAddr;
	unsigned int *kernelIrqAddr;                                             
        unsigned int swiInstrOne;                                           
        unsigned int swiInstrTwo;
	unsigned int irqInstrOne;                                           
        unsigned int irqInstrTwo;  
	volatile int status;

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
	*((unsigned int*)kernelIrqAddr + 1) = (unsigned int)((unsigned int*)I_Handler);                  

	/* Initialize timer and IRQ stack */
	clock = 0;
	setIRQStack(IRQ_SP);
	printf("Setup timer\n");
	
	reg_write(OSTMR_OSMR_ADDR(0), OFFSET);	      // Set match register to desired offset
	reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);      // Set OSMR0 match register to active
	reg_clear(INT_ICLR_ADDR, (1<<INT_OSTMR_0));   // Ensure that timer interrupt is always IRQ 
	reg_set(INT_ICMR_ADDR, (1<<INT_OSTMR_0));     // reg_set(INT_ICMR_ADDR, INT_OSTMR_0);
	reg_write(OSTMR_OSCR_ADDR, 0x0); 	      // Reset counter

	printf("calling set userConditions\n");
	/* Set up user space and jump to user function */
	status = setUserConditions(argc, argv, USER_ADDR);	
	
	printf("exit status = %x\n", status);
	/* Restore U-Boot's SWI Handler */
	*kernelSwiAddr = swiInstrOne;                                          
        *((unsigned int*)kernelSwiAddr + 1) = swiInstrTwo;
	*kernelIrqAddr = irqInstrOne;                                          
        *((unsigned int*)kernelIrqAddr + 1) = irqInstrTwo; 
                   
	printf("time %x\n",clock);
	asm volatile("mov r0, %[rtn]": :[rtn] "r" (status));
	//asm volatile("ldr r1, =%[rtn]": :[rtn] "r" (status));	
	asm volatile("ldr r4, =linkR");
	asm volatile("ldr sp, [r4]");
	asm volatile("ldmfd sp!, {r4-r11,lr}");

	asm volatile("mov %[result], r0": [result] "=r" (status) : );
	printf("returning to uboot\n");

	return status;
}
