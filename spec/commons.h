#ifndef _COMMONS_H_
#define _COMMONS_H_

#define LSB_1b(x) ((x) & 0xFF)
#define MSB_1b(x) (((x) >> 8) & 0xFF)

#define LSB_2b(x) ((x) & 0xFFFF)
#define MSB_2b(x) ((x >> 16) & 0xFFFF)

#define GATE_FLAG_DEFAULT 0x8F00

typedef unsigned short uint16_t;

typedef struct {
  uint16_t offset_low16;
  uint16_t seg_selector;
  uint16_t flags;
  uint16_t offset_high16;
} trap_gate_t;

#endif // _COMMONS_H_