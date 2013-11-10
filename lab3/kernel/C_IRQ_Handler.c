/*
 * Matthew Tay mhtay@andrew.cmu.edu	
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 */

#include <types.h>
#include <arm/reg.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <exports.h>

#define RESOLUTION 100
#define OFFSET 36864

extern volatile unsigned long clock;

/* Called by assembly I_Handler */
void C_IRQ_Handler() {
	printf("IRQ!\n");
	/* Find which interrupt flags are set */
	uint32_t isTimerInt = reg_read(INT_ICPR_ADDR);
	/* Check if timer interrupt flag is set */
	if (((isTimerInt >> INT_OSTMR_0) & 0x01)) {
		reg_write(OSTMR_OSCR_ADDR,0x0); // should be a clear
		reg_set(OSTMR_OSSR_ADDR, OSTMR_OIER_E0);
	
		clock++;
	}
}
	

