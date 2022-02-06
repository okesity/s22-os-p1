/** @file game.c
 *  @brief A kernel with timer, keyboard, console support
 *
 *  This file contains the kernel's main() function.
 *
 *  It sets up the drivers and starts the game.
 *
 *  @author Michael Berman (mberman)
 *  @bug No known bugs.
 */

#include <p1kern.h>

/* Think about where this declaration
 * should be... probably not here!
 */
void tick(unsigned int numTicks);

/* libc includes. */
#include <stdio.h>
#include <simics.h>                 /* lprintf() */
#include <malloc.h>

/* multiboot header file */
#include <multiboot.h>              /* boot_info */

/* memory includes. */
#include <lmm.h>                    /* lmm_remove_free() */

/* x86 specific includes */
#include <x86/seg.h>                /* install_user_segs() */
#include <x86/interrupt_defines.h>  /* interrupt_setup() */
#include <x86/asm.h>                /* enable_interrupts() */

#include <string.h>

#include <p1kern.h>

volatile static int __kernel_all_done = 0;

void test_console() {
    clear_console();
    MAGIC_BREAK;

    show_cursor();
    MAGIC_BREAK;

    hide_cursor();
    MAGIC_BREAK;

    set_cursor(1, 1);
    show_cursor();
    MAGIC_BREAK;

    lprintf( "set color\n" );
    set_term_color(FGND_GREEN | BGND_BLACK);
    MAGIC_BREAK;

    putbyte('a');
    MAGIC_BREAK;

    putbyte('\n');
    putbytes("abcd", 4);
    MAGIC_BREAK;

    putbyte('\b');
    MAGIC_BREAK;

    putbyte('\r');
    MAGIC_BREAK;

    putbyte('A');
    MAGIC_BREAK;

    set_term_color(0xffff);
    MAGIC_BREAK;

    set_cursor(CONSOLE_HEIGHT - 1, 0);
    putbytes("hahahahahahahaha", 16);
    MAGIC_BREAK;

    putbyte((char)'\n');
    MAGIC_BREAK;

    char* verylong = "A Very long str A Very long str A Very long strA Very long \
    strA Very long strA Very long strA Very long strA Very long str \
    strA Very long strA Very long strA Very long strA Very long str \
    strA Very long strA Very long strA Very long strA Very long str";
    putbytes(verylong, strlen(verylong));
    MAGIC_BREAK;

    int color;
    get_term_color(&color);
    lprintf("current color %d, preset culor %d", color, FGND_GREEN | BGND_BLACK);
    MAGIC_BREAK;

    int row, col;
    get_cursor(&row, &col);
    lprintf("cursor: %d, %d", row, col);
    MAGIC_BREAK;

    clear_console();
    get_cursor(&row, &col);
    lprintf("cursor: %d, %d", row, col);
    MAGIC_BREAK;
}

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
    handler_install(tick);

    /*
     * When kernel_main() begins, interrupts are DISABLED.
     * You should delete this comment, and enable them --
     * when you are ready.
     */

    lprintf( "Hello from a brand new kernel!" );

    lprintf("testing console\n");
    test_console();

    while (!__kernel_all_done) {
        continue;
    }

    return 0;
}

/** @brief Tick function, to be called by the timer interrupt handler
 * 
 *  In a real game, this function would perform processing which
 *  should be invoked by timer interrupts.
 *
 **/
void tick(unsigned int numTicks)
{
}
