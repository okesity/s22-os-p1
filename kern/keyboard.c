#include <p1kern.h>
#include <stdio.h>
#include <keyboard.h>
#include <keyhelp.h>
#include <seg.h>
#include <algorithm.h>
#include <interrupt_defines.h>
#include <asm.h>

#define BUFFER_SIZE 512
static uint8_t data[BUFFER_SIZE] = {0};
static size_t wr_index = 0;
static size_t rd_index = 0;

void init_keyboard() {

  // setting the IDT entry
  void* idt_base_addr = idt_base();
  trap_gate_t* trap = (trap_gate_t*) idt_base_addr + KEY_IDT_ENTRY;

  uint32_t keyboard_asm_handler_addr = (uint32_t) keyboard_asm_handler;

  trap->offset_high16 = MSB_16(keyboard_asm_handler_addr);
  trap->offset_low16 = LSB_16(keyboard_asm_handler_addr);
  trap->flags = GATE_P_MASK;
  trap->seg_selector = SEGSEL_KERNEL_CS;
}

void keyboard_c_handler() {
  data[wr_index] = inb(KEYBOARD_PORT);
  wr_index = (wr_index + 1) % BUFFER_SIZE;

  // ack
  outb(INT_CTL_PORT, INT_ACK_CURRENT);
}


/** @brief Returns the next character in the keyboard buffer
 *
 *  This function does not block if there are no characters in the keyboard
 *  buffer
 *
 *  @return The next character in the keyboard buffer, or -1 if the keyboard
 *          buffer is currently empty
 **/
int readchar(void) {

  disable_interrupts();
  while (rd_index != wr_index) {
    kh_type k = process_scancode(data[rd_index]);
    rd_index = (rd_index + 1) % BUFFER_SIZE;

    if (KH_HASDATA(k) && KH_ISMAKE(k)) {
      enable_interrupts();
      return KH_GETCHAR(k);
    }
  }

  enable_interrupts();
  return -1;
}

/** @brief Reads a line of characters into a specified buffer
 *
 * If the keyboard buffer does not already contain a line of input,
 * readline() will spin until a line of input becomes available.
 *
 * If the line is smaller than the buffer, then the complete line,
 * including the newline character, is copied into the buffer. 
 *
 * If the length of the line exceeds the length of the buffer, only
 * len characters should be copied into buf.
 *
 * Available characters should not be committed into buf until
 * there is a newline character available, so the user has a
 * chance to backspace over typing mistakes.
 *
 * While a readline() call is active, the user should receive
 * ongoing visual feedback in response to typing, so that it
 * is clear to the user what text line will be returned by
 * readline().
 *
 *  @param buf Starting address of buffer to fill with a text line
 *  @param len Length of the buffer
 *  @return The number of characters in the line buffer,
 *          or -1 if len is invalid or unreasonably large.
 **/
int
readline(char *buf, int len)
{
  volatile int *p = (int *)0x4; // The course staff has special permission to look here.
  int initial = *p;

  while (*p == initial)
	continue;

  return -1;
}
