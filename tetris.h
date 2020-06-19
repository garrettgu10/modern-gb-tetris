#ifndef TETRIS_H
#define TETRIS_H
#include <gb/gb.h>
#include <rand.h>
#include "debug.h"
#include "stdio.h"

extern UINT8 board[400];

#define tetris_board(x, y) (board[10*(39 - (y)) + x])
#define tetris_row(y) (board + 10 * (39 - (y)))
#define T 0
#define O 1
#define J 2
#define L 3
#define Z 4
#define S 5
#define I 6

#define CLOCKWISE 0
#define COUNTERCW 1

extern INT8 curr_piece_x, curr_piece_y;
extern UINT8 curr_piece_type;

extern UINT8 curr_piece[8];
extern UINT8 curr_rotation_index;
extern UINT8 curr_level;
extern UINT8 held_piece;

extern UINT8 lock_delay_active;
extern UINT8 lock_delay_max;
extern UINT8 lock_delay_counter;
extern UINT8 lock_delay_extensions;
extern UINT8 lock_delay_extensions_max;
extern INT8 lock_delay_extensions_min_y;

UINT8 check_collision(UINT8 x, UINT8 y);
void copy_piece(UINT8 *src, UINT8 *dest);
void set_curr_piece(UINT8 type);
void rotate(UINT8 direction);

void new_bag();
UINT8 get_next_piece();
void show_piece_previews();

void clear_lines();

void show_held_piece();
void swap_piece();

void tetris_init();

void tetris_show_board();

UINT8 check_collision(UINT8 x, UINT8 y);

void hard_drop();

void show_curr_piece();
void show_ghost_piece();

#define LEFT -1
#define RIGHT 1
#define DOWN 0
void move_piece(INT8 direction);

void handle_lock_delay();
void handle_gravity();

#endif
