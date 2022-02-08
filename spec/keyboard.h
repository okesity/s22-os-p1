#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

/**
 * @brief ready the keyboard interrupt by putting its asm handler
 * into the IDT table, location indicated by KEY_IDT_ENTRY
 * 
 */
void init_keyboard();

/**
 * @brief C handler for keyboard interrupt
 *
 */
void keyboard_c_handler();

/**
 * @brief ASM handler for keyboard interrupts
 * 
 *  Save and restore all the general register,
 *  and call the keyboard C handler in between
 */
void keyboard_asm_handler();

#endif /* _KEYBOARD_H_ */