#include "scoring.h"
#include <gb/gb.h>
#include "tetris.h"

BCD ZERO = MAKE_BCD(0);
BCD score = MAKE_BCD(0);
BCD line_count = MAKE_BCD(0);
BCD level_line_count = MAKE_BCD(0); // count up to 20

unsigned char buf[10];
UINT8 score_len;
UINT8 line_count_len;
BCD level_bcd;
UINT8 level_len;
void draw_scores() {
    bcd2text(&score, 47, buf);
    score_len = bcd_len(&score);
    set_bkg_tiles(6 - score_len, 5, score_len, 1, buf + 8 - score_len);

    bcd2text(&line_count, 47, buf);
    line_count_len = bcd_len(&line_count);
    set_bkg_tiles(6 - line_count_len, 7, line_count_len, 1, buf + 8 - line_count_len);

    level_bcd = BCD_HEX(curr_level / 10 * 16 + curr_level % 10);
    bcd2text(&level_bcd, 47, buf);
    level_len = bcd_len(&level_bcd);
    set_bkg_tiles(6 - level_len, 9, level_len, 1, buf + 8 - level_len);
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

    draw_scores();
}