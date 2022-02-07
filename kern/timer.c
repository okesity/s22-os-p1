
#include <timer_defines.h>
#include <asm.h>
#include <interrupt_defines.h>
#include <timer.h>
#include <seg.h>
#include <algorithm.h>
#include <p1kern.h>

#include <simics.h>                 /* lprintf() */

static unsigned int g_numTicks = 0;
static void (*g_timer_callback)(unsigned int) = 0;

void init_timer(void (*tickback)(unsigned int)) {

  g_timer_callback = tickback;

  // setting the IDT entry
  void* idt_base_addr = idt_base();
  trap_gate_t* trap = (trap_gate_t*) idt_base_addr + TIMER_IDT_ENTRY;

  uint32_t timer_asm_handler_addr = (uint32_t) timer_asm_handler;
  trap->offset_high16 = MSB_16(timer_asm_handler_addr);
  trap->offset_low16 = LSB_16(timer_asm_handler_addr);
  trap->flags = GATE_P_MASK;
  trap->seg_selector = SEGSEL_KERNEL_CS;

  
  // initialize timer mode and rate
  outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
  outb(TIMER_PERIOD_IO_PORT, LSB_8(TIMER_RATE));
  outb(TIMER_PERIOD_IO_PORT, MSB_8(TIMER_RATE));
}


void timer_c_handler() {
  g_numTicks++;
  if (g_timer_callback != 0)
    (*g_timer_callback)(g_numTicks);

  // ack
  outb(INT_CTL_PORT, INT_ACK_CURRENT);

}