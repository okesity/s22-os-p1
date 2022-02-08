#include <p1kern.h>
#include <stdio.h>
#include <keyboard.h>
#include <keyhelp.h>
#include <seg.h>
#include <commons.h>
#include <interrupt_defines.h>
#include <asm.h>

#include <string.h>

// the maximum number of charactors that can be displayed on the screen
#define BUFFER_SIZE 2000

// data is a circular buffer with size BUFFER_SIZE
// when there is no data, wr_index == rd_index
// the indexes would only increase or wrap around at the end
static uint8_t data[BUFFER_SIZE] = {0};
static size_t wr_index = 0;
static size_t rd_index = 0;

// const string for indication when readline()
static const char* READLINE_INDICATOR = "\nInput:\n";

/**
 * @brief ready the keyboard interrupt by putting its asm handler
 * into the IDT table, location indicated by KEY_IDT_ENTRY
 * 
 */
void init_keyboard() {

  // setting the IDT entry
  void* idt_base_addr = idt_base();

  trap_gate_t* trap = (trap_gate_t*) idt_base_addr + KEY_IDT_ENTRY;

  uint32_t keyboard_asm_handler_addr = (uint32_t) keyboard_asm_handler;
  trap->offset_high16 = MSB_2b(keyboard_asm_handler_addr);
  trap->offset_low16 = LSB_2b(keyboard_asm_handler_addr);
  trap->flags = GATE_FLAG_DEFAULT;
  trap->seg_selector = SEGSEL_KERNEL_CS;
}

/**
 * @brief C handler for keyboard interrupt
 *
 *    Puts the scancode into the buffer and increase the wr_index
 * 
 */
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
 *  There is no need to disable interrupts here, because the keyboard interrupt
 *  will only modify the wr_index, while readchar only modify the rd_index
 *
 *  @return The next character in the keyboard buffer, or -1 if the keyboard
 *          buffer is currently empty
 **/
int readchar(void) {

  while (rd_index != wr_index) {
    // keep looking until we have a valid charactor or 
    // there are no new data in the buffer
    kh_type k = process_scancode(data[rd_index]);
    rd_index = (rd_index + 1) % BUFFER_SIZE;

    if (KH_HASDATA(k) && KH_ISMAKE(k)) {
      // return the valid charactor
      return KH_GETCHAR(k);
    }
  }

  return -1;
}

/** @brief Reads a line of characters into a specified buffer
 *
 * readline() will spin until a line of input is available
 * and return the new line if either it received the \n character
 * or it has read len characters
 *
 *
 *  @param buf Starting address of buffer to fill with a text line
 *  @param len Length of the buffer
 *  @return The number of characters in the line buffer,
 *          or -1 if len is invalid or unreasonably large.
 **/
int readline(char *buf, int len) {
  if (len >= BUFFER_SIZE || len < 0)
    return -1;

  int len_read = 0, ch = -1;

  // prints the readline indicator to console
  putbytes(READLINE_INDICATOR, strlen(READLINE_INDICATOR));

  while (len_read < len) {
    ch = readchar();
    if (ch != -1) {
      // printing the current char to console
      putbyte(ch);
      if (ch == '\b' && len_read > 0) {
        len_read--;
      } else if (ch == '\r') {
        len_read = 0;
      } else {
        buf[len_read++] = ch;
        if (ch == '\n')
          break;
      }
    } 
  }

  return len_read;
}

