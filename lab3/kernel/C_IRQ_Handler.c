/*
 * Matthew Tay mhtay@andrew.cmu.edu	
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 */

#include <types.h>
#include <arm/reg.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#define RESOLUTION 10

extern volatile unsigned long timer;

/* Called by assembly I_Handler */
void C_IRQ_Handler() {
	/* Find which interrupt flags are set */
	uint32_t isTimerInt = reg_read(INT_ICPR_ADDR);
	/* Check if timer interrupt flag is set */
	isTimerInt = (isTimerInt >> (INT_OSTMR_0 - 1)) & 0x01;
	if (isTimerInt) {
		/* Find current time */
		uint32_t nextIntTime = reg_read(OSTMR_OSMR_ADDR(0));
		/* Add desired offset */
		nextIntTime += (msToCycles(RESOLUTION)); // Find way to determine next time desired
		/* Set next interrupt time */
		reg_write(OSTMR_OSMR_ADDR(0), nextIntTime);
		/* Acknowledge match occured and clear flag */
		reg_set(OSTMR_OSSR_ADDR, 0x1);

		/* Offset between next timer interrupt determines resolution */
		timer++;
	}
}
