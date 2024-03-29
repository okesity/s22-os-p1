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

void wait_char(char testc){
  char c;
  do{
    c = readchar();
  }while(c != testc);
}

/**
 * @brief Test cases for the console 
 * 
 */
void test_console() {
    clear_console();

    show_cursor();
    wait_char('c');

    hide_cursor();
    wait_char('c');;

    set_cursor(1, 1);
    show_cursor();
    wait_char('c');;

    lprintf( "set color\n" );
    set_term_color(FGND_GREEN | BGND_BLACK);
    wait_char('c');;

    putbyte('a');
    wait_char('c');;

    putbyte('\n');
    putbytes("abcd", 4);
    wait_char('c');;

    putbyte('\b');
    wait_char('c');;

    putbyte('\r');
    wait_char('c');;

    putbyte('A');
    wait_char('c');;

    set_term_color(0xffff); // invalid color
    wait_char('c');;

    set_cursor(CONSOLE_HEIGHT - 1, 0);
    putbytes("hahahahahahahaha", 16);
    wait_char('c');;

    putbyte((char)'\n');
    wait_char('c');;

    char* verylong = "A Very long str A Very long str A Very long strA Very long \
    strA Very long strA Very long strA Very long strA Very long str \
    strA Very long strA Very long strA Very long strA Very long str \
    strA Very long strA Very long strA Very long strA Very long str";
    putbytes(verylong, strlen(verylong));
    wait_char('c');;

    int color;
    get_term_color(&color);
    lprintf("current color %d, preset culor %d", color, FGND_GREEN | BGND_BLACK);
    wait_char('c');;

    int row, col;
    get_cursor(&row, &col);
    lprintf("cursor: %d, %d", row, col);
    wait_char('c');;

    clear_console();
    get_cursor(&row, &col);
    lprintf("cursor: %d, %d", row, col);
    wait_char('c');;
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
    enable_interrupts();
    /*
     * When kernel_main() begins, interrupts are DISABLED.
     * You should delete this comment, and enable them --
     * when you are ready.
     */

    lprintf( "Hello from a brand new kernel!" );

    lprintf("testing console\n");
    test_console();

    MAGIC_BREAK;
    wait_char('c'); putbyte('d');
    wait_char('c'); putbyte('d');
    wait_char('c'); putbyte('d');
    MAGIC_BREAK;

    lprintf("testing readline\n");

    while (!__kernel_all_done) {
      char buf[20] = {0};
      int rv = readline(buf, 20);
      lprintf("read line rv %d\n", rv);

      printf("%s",buf);
      MAGIC_BREAK;
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
