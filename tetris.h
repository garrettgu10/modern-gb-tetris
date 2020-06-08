#ifndef TETRIS_H
#define TETRIS_H
#include <gb/gb.h>
#include <rand.h>
#include "globals.h"
#include "stdio.h"

UINT8 board[200];

#define tetris_board(x, y) (board[10*(19 - (y)) + x])

UINT8 pieces[56] = {
     //T piece
        0, 1, 1, 1, 2, 1, 1, 2,
     //O piece
        0, 0, 0, 1, 1, 0, 1, 1,
     //J piece
        0, 1, 1, 1, 2, 1, 0, 2,
     //L piece
        0, 1, 1, 1, 2, 1, 2, 2,
     //Z piece
        0, 2, 1, 2, 1, 1, 2, 1,
     //S piece
        0, 1, 1, 1, 1, 2, 2, 2,
     //I piece
        0, 2, 1, 2, 2, 2, 3, 2
};

UINT8 NORMAL_CLOCKWISE_WALL_KICKS[40] = {
    0, 0, -1, 0, -1, 1, 0, -2, -1, -2,
    0, 0, 1, 0, 1, -1, 0, 2, 1, 2,
    0, 0, 1, 0, 1, 1, 0, -2, 1, -2,
    0, 0, -1, 0, -1, -1, 0, 2, -1, -2
};

UINT8 bounding_box_size[7] = {3, 2, 3, 3, 3, 3, 4}; //width and height

UINT8 current_piece_position[2];
UINT8 current_piece_type;

UINT8 current_piece[8];
UINT8 current_rotation_index = 0;

UINT8 check_collision(UINT8 x, UINT8 y);

UINT8 i2;
void set_current_piece(UINT8 type) {
    for(i2 = 0; i2 < 8; i2++){
        current_piece[i2] = pieces[8*type + i2];
    }
    current_piece_type = type;
    current_rotation_index = 0;
    current_piece_position[0] = 5 - bounding_box_size[current_piece_type] / 2;
    current_piece_position[1] = 15;
}

UINT8 i3, i4, x1, y1;
UINT8 wall_kick_index, kicked_x, kicked_y;
UINT8 old_piece[8];
void rotate_clockwise() {
    //make a copy of the old piece
    for(i4 = 0; i4 < 8; i4++){
        old_piece[i4] = current_piece[i4];
    }

    //rotate the current piece
    for(i3 = 0; i3 < 4; i3++) {
        x1 = current_piece[2*i3 + 1];
        y1 = bounding_box_size[current_piece_type] - 1 - current_piece[2*i3];
        
        current_piece[2*i3] = x1;
        current_piece[2*i3 + 1] = y1;
    }

    //chcek srs
    for(i3 = 0; i3 < 5; i3++){
        wall_kick_index = i3 * 2 + current_rotation_index * 10;

        kicked_x = current_piece_position[0] + NORMAL_CLOCKWISE_WALL_KICKS[wall_kick_index];
        kicked_y = current_piece_position[1] + NORMAL_CLOCKWISE_WALL_KICKS[wall_kick_index + 1];

        if(!check_collision(kicked_x, kicked_y)) {
            current_piece_position[0] = kicked_x;
            current_piece_position[1] = kicked_y;
            current_rotation_index = (current_rotation_index + 1) % 4;
            return;
        }
    }

    //wall kick failed, do not rotate
    for(i4 = 0; i4 < 8; i4++){
        current_piece[i4] = old_piece[i4];
    }
}

UINT8 tetris_bag[7];
UINT8 i5;
UINT8 t3;
UINT8 exchange_index;
void new_bag() {
    for(i5 = 0; i5 < 7; i5++){
        tetris_bag[i5] = i5;
    }

    for(i5 = 0; i5 < 7; i5++){
        t3 = tetris_bag[i5];
        exchange_index = i5 + (rand() % (7 - i5));
        tetris_bag[i5] = tetris_bag[exchange_index];
        tetris_bag[exchange_index] = t3;
    }
}

UINT8 current_index = 0;
UINT8 get_next_piece() {
    if(current_index == 7) {
        current_index = 0;
        new_bag();
    }
    return tetris_bag[current_index++];
}

UINT8 x2, y2, i6;
UINT8 has_hole;
void clear_lines() {
    for(y2 = 0; y2 < 20; y2++) {
        has_hole = 0;
        for(x2 = 0; x2 < 10; x2++){
            if(tetris_board(x2, y2) == 0) {
                has_hole = 1;
            }
        }

        if(!has_hole) {
            //move every line above this down
            for(i6 = y2; i6 < 19; i6++) {
                for(x2 = 0; x2 < 10; x2++){
                    tetris_board(x2, i6) = tetris_board(x2, i6+1);
                }
            }
            y2--;
        }
    }
}

UINT8 held_piece = 255;
void swap_piece() {
    if(held_piece == 255) {
        held_piece = current_piece_type;
    }
}

void tetris_init() {
    new_bag();
    set_current_piece(get_next_piece());
}

void tetris_show_board() {
    set_bkg_tiles(2, 0, 10, 18, board+20);
}

UINT8 has_collision;
UINT8 i1;
UINT8 cc_x, cc_y, cc_t;
UINT8 check_collision(UINT8 x, UINT8 y) {

    for(i1 = 0; i1 < 4; i1++){
        cc_x = x + current_piece[2*i1];
        cc_y = y + current_piece[2*i1 + 1];
        
        if(tetris_board(cc_x, cc_y) != 0) {
            return 1;
        }

        if(cc_x > 9 || cc_y > 20) {
            return 1;
        }
    }

    return 0;
}

UINT8 curr_pos_x;
UINT8 curr_pos_y;
UINT8 new_x, new_y;
UINT8 colliding;
void hard_drop() {
    for(i = current_piece_position[1]; ; i--) {å
        if(check_collision(current_piece_position[0], i) != 0) {
            i++;
            break;
        }
    }
    
    new_x = current_piece_position[0];
    new_y = i;
    
    for(i = 0; i < 4; i++){
        curr_pos_x = new_x + current_piece[2*i];
        curr_pos_y = new_y + current_piece[2*i + 1];

        tetris_board(curr_pos_x, curr_pos_y) = current_piece_type + 1;
    }

    set_current_piece(get_next_piece());

    clear_lines();

    tetris_show_board();
}

void tetris_show_current_piece() {
    set_sprite_tile(0, current_piece_type);
    for(i = 0; i < 4; i++){
        set_sprite_tile(i, current_piece_type);
        
        curr_pos_x = current_piece_position[0] + current_piece[2*i];
        curr_pos_y = current_piece_position[1] + current_piece[2*i + 1];

        move_sprite(i, 24U + (curr_pos_x << 3), 152U - (curr_pos_y << 3));
    }
}

#endif
