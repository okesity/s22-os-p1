#ifndef _TIMER_H_
#define _TIMER_H_

void init_timer(void (*tickback)(unsigned int));
void timer_c_handler();
void timer_asm_handler();

#endif /* _TIMER_H_ */