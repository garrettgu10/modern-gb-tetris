//https://github.com/untoxa/example_bcd/

#ifndef __BCD_H_INCLUDE
#define __BCD_H_INCLUDE

#include <asm/types.h>

// macro for creating BCD constants
#define BCD_HEX(v) ((BCD)(v))
#define MAKE_BCD(v) BCD_HEX(0x ## v)

typedef unsigned long BCD;

// adding BCD numbers: sour += value
void bcd_add(BCD * sour, const BCD * value);
// subtracting BCD numbers: sour -= value
void bcd_sub(BCD * sour, const BCD * value);
// compare two BCD numbers: returns 1 if greater, 0 if equal, -1 if lower
BYTE bcd_compare(const BCD * A, const BCD * B);
UINT8 bcd_len(const BCD * A);

// convert the BCD number into asciiz string and return the length
UBYTE bcd2text(const BCD * bcd, UBYTE tile_offset, unsigned char * buffer);

#endif