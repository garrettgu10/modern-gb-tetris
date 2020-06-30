#include "scoring.h"
#include <gb/gb.h>
#include "tetris.h"
#include "text.h"

BCD ZERO = MAKE_BCD(0);
BCD score = MAKE_BCD(0);
BCD line_count = MAKE_BCD(0);
BCD level_line_count = MAKE_BCD(0); // count up to 20

void reset_scoring() {
    score = MAKE_BCD(0);
    line_count = MAKE_BCD(0);
    level_line_count = MAKE_BCD(0);
}

unsigned char buf[10];
UINT8 score_len;
UINT8 line_count_len;
BCD level_bcd;
UINT8 level_len;
void draw_scores() {
    write_text("SCORE", 1, 5);
    bcd2text(&score, 47, buf);
    score_len = bcd_len(&score);
    set_bkg_tiles(6 - score_len, 6, score_len, 1, buf + 8 - score_len);

    write_text("LINES", 1, 8);
    bcd2text(&line_count, 47, buf);
    line_count_len = bcd_len(&line_count);
    set_bkg_tiles(6 - line_count_len, 9, line_count_len, 1, buf + 8 - line_count_len);

    write_text("LEVEL", 1, 11);
    level_bcd = BCD_HEX(curr_level / 10 * 16 + curr_level % 10);
    bcd2text(&level_bcd, 47, buf);
    level_len = bcd_len(&level_bcd);
    set_bkg_tiles(6 - level_len, 12, level_len, 1, buf + 8 - level_len);
}

BCD temp;
BCD LINES_PER_LEVEL = MAKE_BCD(10);
void count_lines(UINT8 lines, UINT8 is_tspin, UINT8 is_pclear) {
    temp = BCD_HEX(lines);
    bcd_add(&line_count, &temp);
    bcd_add(&level_line_count, &temp);

    if(curr_level != 25 && bcd_compare(&level_line_count, &LINES_PER_LEVEL) >= 0) {
        bcd_sub(&level_line_count, &LINES_PER_LEVEL);
        curr_level++;
    }

    if(lines == 1) {
        show_toast("SINGLE", 1);
    }else if(lines == 2) {
        show_toast("DOUBLE", 1);
    }else if(lines == 3){
        show_toast("TRIPLE", 1);
    }else if(lines == 4){
        show_toast("TETRIS", 1);
    }

    if(is_pclear) {
        show_toast("PCLEAR", 3);
    }

    if(is_tspin) {
        show_toast("T SPIN", 0);
    }

    draw_scores();
}

#define FRAMES_PER_TOAST 255

UINT8 toast_frames_remaining[4] = {0}; // for each toast
//y: 14, 15, 16, 17
void show_toast(char* msg, UINT8 toast_slot) { //toast_slot: [0, 3]
    write_text(msg, 0, 14 + toast_slot);
    toast_frames_remaining[toast_slot] = FRAMES_PER_TOAST;
}

UINT8 toast_i;
void handle_toast_frame() {
    for(toast_i = 0; toast_i < 4; toast_i++){
        if(toast_frames_remaining[toast_i] == 0) {
            continue;
        }

        toast_frames_remaining[toast_i]--;

        if(toast_frames_remaining[toast_i] == 0) {
            write_text("      ", 0, 14 + toast_i);
        }
    }
}