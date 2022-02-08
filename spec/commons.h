#ifndef _COMMONS_H_
#define _COMMONS_H_

/* Useful algorithms and bit manipulations  */

#define LSB_1b(x) ((x) & 0xFF)
#define MSB_1b(x) (((x) >> 8) & 0xFF)

#define LSB_2b(x) ((x) & 0xFFFF)
#define MSB_2b(x) (((x) >> 16) & 0xFFFF)

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

/* Default value for flag fields in a trap gate  */
#define GATE_FLAG_DEFAULT 0x8F00

typedef unsigned short uint16_t;

/**
 * @brief struct for a trap gate int the 
 * IDT entry, total size 32 bit
 * 
 * Starting from the 0-th bit, the trap gate
 * consits of:
 *  - lower 16 bit of the handler entry point
 *  - 16 bit segment selector for destination 
 *      code segment
 *  - 16 bit flags including P, DPL and others
 *  - higher 16 bit of the handler entry point
 * 
 */
typedef struct {
  uint16_t offset_low16;
  uint16_t seg_selector;
  uint16_t flags;
  uint16_t offset_high16;
} trap_gate_t;

#endif // _COMMONS_H_