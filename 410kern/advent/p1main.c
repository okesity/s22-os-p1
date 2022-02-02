/** @file p1main.c
 *  @brief Startup code for Adventure
 *
 *  This file contains the kernel's main() function.
 *  It sets up the drivers and starts the game.
 *
 *  @author Michael Berman (mberman)
 *  @author Cyrus Bomi Daruwala (cdaruwal)
 *  @bug No known bugs.
 */

#include <p1kern.h>

/* libc includes. */
#include <stdio.h>
#include <stdlib.h>
#include <simics.h>                 /* lprintf() */
#include <malloc.h>
#include <assert.h>

/* multiboot header file */
#include <multiboot.h>              /* boot_info */

/* memory includes. */
#include <lmm.h>                    /* lmm_remove_free() */

/* x86 specific includes */
#include <x86/seg.h>                /* install_user_segs() */
#include <x86/interrupt_defines.h>  /* interrupt_setup() */
#include <x86/asm.h>                /* enable_interrupts() */

#include <string.h>

#include "extern.h"

/** @brief Kernel entrypoint.
 *  
 *  This is the entrypoint for the kernel.  It simply sets up the
 *  drivers and passes control off to game_run().
 *
 * @return Does not return
 */
int kernel_main(mbinfo_t *mbinfo, int argc, char **argv, char **envp)
{
    /*
     * Initialize device-driver library.
     */
    if(handler_install(tick) < 0) {
      panic("oh noes! where are my handlers");
    }
    enable_interrupts();
    lprintf( "Welcome to Adventure!!  Would you like instructions?" );

    clear_console();
    game_main(0, NULL);

    return -9;
}
