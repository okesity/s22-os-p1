
#include <timer_defines.h>
#include <asm.h>
#include <interrupt_defines.h>
#include <timer.h>
#include <seg.h>
#include <commons.h>
#include <p1kern.h>

#define TIMER_INT_RATE 11931      // rate for generating 10 milliseconds interrupts


static unsigned int numTicks = 0;   // current numTick record
static void (*timer_callback)(unsigned int) = 0; // callback specified by application

/**
 * @brief ready the timer interrupt by putting its asm handler
 * into the IDT table, location indicated by TIMER_IDT_ENTRY
 *
 *  Initialize the timer at a rate defined by TIMER_INT_RATE
 * 
 */
void init_timer(void (*tickback)(unsigned int)) {

  timer_callback = tickback;

  // setting the IDT entry
  void* idt_base_addr = idt_base();
  trap_gate_t* trap = (trap_gate_t*) idt_base_addr + TIMER_IDT_ENTRY;

  uint32_t timer_asm_handler_addr = (uint32_t) timer_asm_handler;
  trap->offset_high16 = MSB_2b(timer_asm_handler_addr);
  trap->offset_low16 = LSB_2b(timer_asm_handler_addr);
  trap->flags = GATE_FLAG_DEFAULT;
  trap->seg_selector = SEGSEL_KERNEL_CS;

  
  // initialize timer mode and rate
  outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
  outb(TIMER_PERIOD_IO_PORT, LSB_1b(TIMER_INT_RATE));
  outb(TIMER_PERIOD_IO_PORT, MSB_1b(TIMER_INT_RATE));
}

/**
 * @brief C handler for timer interrupt
 *
 *  Increment the numTick and passed it to 
 *  the callback function 
 * 
 */
void timer_c_handler() {
  numTicks++;
  if (timer_callback != 0)
    (*timer_callback)(numTicks);

  // ack
  outb(INT_CTL_PORT, INT_ACK_CURRENT);
}