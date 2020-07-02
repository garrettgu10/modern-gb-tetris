
#include "machine.h"

void bzero(UINT16 sz, void *buf) {
/*    sz; buf;
    __asm
        LDA HL, 4(SP)
        LD E, (HL) ; DE -- length
        INC HL
        LD D, (HL)
        INC HL

        LD A, (HL+)
        LD H, (HL)
        LD L, A ; HL --  buffer

        LD A, #0x0

    bzero_loop_begin:
        CP E
        JP NZ, bzero_handle_nonzero
        CP D
        JP NZ, bzero_handle_nonzero
        JP bzero_loop_end

    bzero_handle_nonzero:

        DEC DE
        LD A, (HL+)

        JP bzero_loop_begin

    bzero_loop_end:
        RET
    __endasm;
*/
}