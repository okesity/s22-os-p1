#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#define LSB_8(x) ((x) & 0xF)
#define MSB_8(x) (((x) >> 8) & 0xF)

#define LSB_16(x) ((x) & 0xFF)
#define MSB_16(x) ((x >> 16) & 0xFF)

#endif // _ALGORITHM_H_