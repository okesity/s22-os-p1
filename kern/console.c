/*********************************************************************/
/*                                                                   */
/* Console interface functions                                       */
/*                                                                   */
/*********************************************************************/

#include <video_defines.h>
#include <asm.h>
#include <p1kern.h>
#include <simics.h>                 /* lprintf() */
#include <string.h>
#include <algorithm.h>

#define COLOR_BIT_LEN 8

static int g_color = FGND_WHITE | BGND_BLACK;
static int g_cursor_x = 0; 
static int g_cursor_y = 0;
static int g_isHidden = 0;

/**
 * @brief scroll down the console by len line
 * 
 * When scrolling down, simply discard the first len line
 * and copy all data starting from len + 1 line
 * 
 * @param len: number of lines to scroll down
 */
void scroll_down(int len) {
  if (len >= CONSOLE_HEIGHT) {
    // set new screen
    clear_console();
  } else {
    g_cursor_x -= len;
    // move up existing contends
    memmove((void*) CONSOLE_MEM_BASE, (void*) (CONSOLE_MEM_BASE + len * 2 * CONSOLE_WIDTH), 
      2 * (CONSOLE_HEIGHT - len) * CONSOLE_WIDTH);
    
    // set new lines to be empty
    memset((void*) (CONSOLE_MEM_BASE + 2 * (CONSOLE_HEIGHT - len) * CONSOLE_WIDTH), 
      0, 2 * len * CONSOLE_WIDTH);
  }
}

void set_crtc(int row, int col) {

  uint16_t data = row * CONSOLE_WIDTH + col;

  outb(CRTC_IDX_REG, CRTC_CURSOR_LSB_IDX);
  outb(CRTC_DATA_REG, LSB_8(data));
    
  outb(CRTC_IDX_REG, CRTC_CURSOR_MSB_IDX);
  outb(CRTC_DATA_REG, MSB_8(data));
}

void move_cursor_next() {
  if (g_cursor_y == CONSOLE_WIDTH - 1) {
    set_cursor(g_cursor_x + 1, 0);
  } else {
    set_cursor(g_cursor_x, g_cursor_y + 1);
  }
}


/** @brief Prints character ch at the current location
 *         of the cursor.
 *
 *  If the character is a newline ('\n'), the cursor is moved
 *  to the beginning of the next line (scrolling if necessary).
 *  If the character is a carriage return ('\r'), the cursor is
 *  immediately reset to the beginning of the current line,
 *  causing any future output to overwrite any existing output
 *  on the line.  If backspace ('\b') is encountered, the previous
 *  character is erased.  See the main console.c description found
 *  on the handout web page for more backspace behavior.
 *
 *  @param ch the character to print
 *  @return The input character
 */
int putbyte( char ch ) {
  if (ch == '\n') {
    set_cursor(g_cursor_x + 1, 0);
  } else if (ch == '\r') {
    set_cursor(g_cursor_x, 0);
  } else if (ch == '\b') {
    if (g_cursor_y == 0) {
      if (g_cursor_x > 0) 
        set_cursor(g_cursor_x - 1, CONSOLE_WIDTH - 1);
    } else {
      set_cursor(g_cursor_x, g_cursor_y - 1);
    }
    draw_char(g_cursor_x, g_cursor_y, '\0', g_color);
  } else {
    draw_char(g_cursor_x, g_cursor_y, ch, g_color);
    move_cursor_next();
  }
  return ch;
}

/** @brief Prints the string s, starting at the current
 *         location of the cursor.
 *
 *  If the string is longer than the current line, the
 *  string fills up the current line and then
 *  continues on the next line. If the string exceeds
 *  available space on the entire console, the screen
 *  scrolls up one line, and then the string
 *  continues on the new line.  If '\n', '\r', and '\b' are
 *  encountered within the string, they are handled
 *  as per putbyte. If len is not a positive integer or s
 *  is null, the function has no effect.
 *
 *  @param s The string to be printed.
 *  @param len The length of the string s.
 *  @return Void.
 */
void putbytes(const char* s, int len) {
  for (int i = 0; i < len; i++) {
    putbyte(s[i]);
  }
}

/** @brief Changes the foreground and background color
 *         of future characters printed on the console.
 *
 *  If the color code is invalid, the function has no effect.
 *
 *  @param color The new color code.
 *  @return 0 on success or integer error code less than 0 if
 *          color code is invalid.
 */
int set_term_color(int color) {
  if ((color >> COLOR_BIT_LEN) != 0) {
    lprintf("color code invalid: %d\n", color);
    return -1;
  }
  g_color = color;
  return 0;
}

/** @brief Writes the current foreground and background
 *         color of characters printed on the console
 *         into the argument color.
 *  @param color The address to which the current color
 *         information will be written.
 *  @return Void.
 */
void get_term_color(int* color) {
  *color = g_color;
}

/** @brief Sets the position of the cursor to the
 *         position (row, col).
 *
 *  Subsequent calls to putbytes should cause the console
 *  output to begin at the new position. If the cursor is
 *  currently hidden, a call to set_cursor() does not show
 *  the cursor.
 *
 *  @param row The new row for the cursor.
 *  @param col The new column for the cursor.
 *  @return 0 on success or integer error code less than 0 if
 *          cursor location is invalid.
 */
int set_cursor(int row, int col) {
  if (row < 0 || col < 0 || col >= CONSOLE_WIDTH) {
    lprintf("cursor pos invalid %d, %d\n", row, col);
    return -1;
  }

  g_cursor_x = row;
  g_cursor_y = col;
  if (g_cursor_x > CONSOLE_HEIGHT - 1)
    scroll_down(g_cursor_x - CONSOLE_HEIGHT + 1);

  if (!g_isHidden)
    set_crtc(row, col);
  return 0;
}

/** @brief Writes the current position of the cursor
 *         into the arguments row and col.
 *  @param row The address to which the current cursor
 *         row will be written.
 *  @param col The address to which the current cursor
 *         column will be written.
 *  @return Void.
 */
void get_cursor(int* row, int* col) {
  *row = g_cursor_x;
  *col = g_cursor_y;
}

/** @brief Hides the cursor.
 *
 *  Subsequent calls to putbytes do not cause the
 *  cursor to show again.
 *
 *  @return Void.
 */
void hide_cursor(void) {
  if (!g_isHidden) {
    g_isHidden = 1;
    set_crtc(CONSOLE_WIDTH, CONSOLE_HEIGHT);
  }
}

/** @brief Shows the cursor.
 *
 *  If the cursor is already shown, the function has no effect.
 *
 *  @return Void.
 */
void show_cursor(void) {
  if (g_isHidden) {
    g_isHidden = 0;
    set_crtc(g_cursor_x, g_cursor_y);
  }
}

/** @brief Clears the entire console.
 *
 * The cursor is reset to the first row and column
 * If the cursor is hidden, stay hidden
 *
 *  @return Void.
 */
void clear_console(void) {
  memset((void*) CONSOLE_MEM_BASE, 0, CONSOLE_HEIGHT * CONSOLE_WIDTH * 2);
  set_cursor(0, 0);
}

/** @brief Prints character ch with the specified color
 *         at position (row, col).
 *
 *  If any argument is invalid, the function has no effect.
 *
 *  @param row The row in which to display the character.
 *  @param col The column in which to display the character.
 *  @param ch The character to display.
 *  @param color The color to use to display the character.
 *  @return Void.
 */
void draw_char(int row, int col, int ch, int color) {
  *(char *)(CONSOLE_MEM_BASE + 2 * (row * CONSOLE_WIDTH + col)) = ch;
  *(char *)(CONSOLE_MEM_BASE + 2 * (row * CONSOLE_WIDTH + col) + 1) = color;  
}

/** @brief Returns the character displayed at position (row, col).
 *  @param row Row of the character.
 *  @param col Column of the character.
 *  @return The character at (row, col).
 */
char get_char(int row, int col) {
  return *(char *)(CONSOLE_MEM_BASE + 2 * (row * CONSOLE_WIDTH + col));
}