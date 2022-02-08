#ifndef _TIMER_H_
#define _TIMER_H_

/**
 * @brief ready the timer interrupt by putting its asm handler
 * into the IDT table, location indicated by TIMER_IDT_ENTRY
 *
 *  Initialize the timer at a rate defined by TIMER_INT_RATE
 * 
 */
void init_timer(void (*tickback)(unsigned int));

/**
 * @brief C handler for keyboard interrupt
 *
 *  Increment the numTick and passed it to 
 *  the callback function 
 * 
 */
void timer_c_handler();

/**
 * @brief ASM handler for Timer Interrupt
 * 
 *  Save and restore all the general register,
 *  and call the timer C handler in between
 */
void timer_asm_handler();

#endif /* _TIMER_H_ */