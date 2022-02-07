
#include <p1kern.h>
#include <stdio.h>
#include <timer.h>
#include <keyboard.h>

int handler_install(void (*tickback)(unsigned int))
{
  init_timer(tickback);
  init_keyboard();
  return 0;
}