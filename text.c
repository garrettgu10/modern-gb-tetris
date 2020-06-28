#include "text.h"
#include <gb/gb.h>

UINT8 write_text_buf[30];
UINT8 strlen;
void write_text(char* str, UINT8 x, UINT8 y) {
    strlen = 0;
    while(*str != 0) {
        if(*str >= 'A' && *str < 'Z'){
            write_text_buf[strlen] = *str - 'A' + 17;
        }else if(*str == ' '){
            write_text_buf[strlen] = 46;
        }else if(*str >= '0' && *str <= '9') {
            write_text_buf[strlen] = *str - '0' + 47;
        }
        strlen++;
        str++;
    }

    set_bkg_tiles(x, y, strlen, 1, write_text_buf);
}