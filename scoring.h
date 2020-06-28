#ifndef SCORING_H
#define SCORING_H
#include "bcd.h"

void draw_scores();
void count_lines(UINT8 lines, UINT8 is_tspin, UINT8 is_pclear);

void show_toast(char *msg, UINT8 toast_slot);
void handle_toast_frame();

#endif