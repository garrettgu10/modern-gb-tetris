#ifndef DEBUG_H
#define DEBUG_H
#include <gb/gb.h>
#include <stdio.h>

static UINT8 debug_buff[256];
static UINT8 *c;

#define debug_printf(...) {\
    sprintf(debug_buff, __VA_ARGS__); \
    c = debug_buff; \
    while(*c != 0) { \
        _io_out = *c; \
        send_byte(); \
        c++; \
    } \
}

#endif
