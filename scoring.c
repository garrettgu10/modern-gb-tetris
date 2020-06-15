#include "scoring.h"
#include <gb/gb.h>
#include "tetris.h"

BCD line_count = MAKE_BCD(0);
BCD level_line_count = MAKE_BCD(0); // count up to 20

unsigned char buf[10];
UINT8 score_len;
BCD level_bcd;
UINT8 level_len;
void draw_score() {
    bcd2text(&line_count, 47, buf);
    score_len = bcd_len(&line_count);
    set_bkg_tiles(6 - score_len, 5, score_len, 1, buf + 8 - score_len);

    level_bcd = BCD_HEX(curr_level);
    bcd2text(&level_bcd, 47, buf);
    level_len = bcd_len(&level_bcd);
    set_bkg_tiles(6 - level_len, 6, level_len, 1, buf + 8 - level_len);
}

BCD temp;
BCD lines_per_level = MAKE_BCD(10);
void count_lines(UINT8 lines) {
    temp = BCD_HEX(lines);
    bcd_add(&line_count, &temp);
    bcd_add(&level_line_count, &temp);

    if(curr_level != 20 && bcd_compare(&level_line_count, &lines_per_level) >= 0) {
        bcd_sub(&level_line_count, &lines_per_level);
        curr_level++;
    }

    draw_score();
}