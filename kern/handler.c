
#include <p1kern.h>
#include <stdio.h>
#include <timer.h>
#include <keyboard.h>

/** @brief The driver-library initialization function
 *
 *   Installs the timer and keyboard interrupt handler.
 *   NOTE: handler_install should ONLY install and activate the
 *   handlers;
 *
 *   @param tickback Pointer to clock-tick callback function
 *
 *   @return A negative error code on error, or 0 on success
 **/
int handler_install(void (*tickback)(unsigned int))
{
  init_timer(tickback);
  init_keyboard();
  return 0;
}