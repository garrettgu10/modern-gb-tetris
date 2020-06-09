#ifndef TETRIS_H
#define TETRIS_H
#include <gb/gb.h>
#include <rand.h>
#include <debug.h>
#include "globals.h"
#include "stdio.h"

UINT8 board[400];

#define tetris_board(x, y) (board[10*(39 - (y)) + x])
#define T 0
#define O 1
#define J 2
#define L 3
#define Z 4
#define S 5
#define I 6

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

UINT8 I_sprites[16] = {
    9, 10, 10, 11,
    6, 7, 7, 8,
    11, 10, 10, 9,
    8, 7, 7, 6
};

UINT8 NORMAL_CLOCKWISE_WALL_KICKS[40] = {
    0, 0, -1, 0, -1, 1, 0, -2, -1, -2,
    0, 0, 1, 0, 1, -1, 0, 2, 1, 2,
    0, 0, 1, 0, 1, 1, 0, -2, 1, -2,
    0, 0, -1, 0, -1, -1, 0, 2, -1, -2
};

UINT8 NORMAL_COUNTERCLOCKWISE_WALL_KICKS[40] = {
    0, 0, 1, 0, 1, 1, 0, -2, 1, -2,
    0, 0, 1, 0, 1, -1, 0, 2, 1, 2,
    0, 0, -1, 0, -1, 1, 0, -2, -1, -2,
    0, 0, -1, 0, -1, -1, 0, 2, -1, 2
};

UINT8 bounding_box_size[7] = {3, 2, 3, 3, 3, 3, 4}; //width and height

UINT8 curr_piece_x, curr_piece_y;
UINT8 curr_piece_type;

UINT8 curr_piece[8];
UINT8 curr_rotation_index = 0;

UINT8 check_collision(UINT8 x, UINT8 y);

UINT8 i2;
void copy_piece(UINT8 *src, UINT8 *dest) {
    for(i2 = 0; i2 < 8; i2++){
        dest[i2] = src[i2];
    }
}

void set_curr_piece(UINT8 type) {
    copy_piece(pieces + 8*type, curr_piece);
    curr_piece_type = type;
    curr_rotation_index = 0;
    curr_piece_x = 5 - (bounding_box_size[curr_piece_type] + 1) / 2;
    curr_piece_y = (curr_piece_type == 1? 16: 15);
}

#define CLOCKWISE 0
#define COUNTERCW 1

UINT8 i3, i4, x1, y1;
UINT8 wall_kick_index, kicked_x, kicked_y;
UINT8 old_piece[8];
void rotate(UINT8 direction) {
    //make a copy of the old piece
    for(i4 = 0; i4 < 8; i4++){
        old_piece[i4] = curr_piece[i4];
    }

    //rotate the curr piece
    for(i3 = 0; i3 < 4; i3++) {
        if(direction == CLOCKWISE){
            x1 = curr_piece[2*i3 + 1];
            y1 = bounding_box_size[curr_piece_type] - 1 - curr_piece[2*i3];
        }else{
            x1 = bounding_box_size[curr_piece_type] - 1 - curr_piece[2*i3 + 1];
            y1 = curr_piece[2*i3];
        }
        
        curr_piece[2*i3] = x1;
        curr_piece[2*i3 + 1] = y1;
    }

    //chcek srs
    for(i3 = 0; i3 < 5; i3++){
        wall_kick_index = i3 * 2 + curr_rotation_index * 10;

        if(direction == CLOCKWISE){
            kicked_x = curr_piece_x + NORMAL_CLOCKWISE_WALL_KICKS[wall_kick_index];
            kicked_y = curr_piece_y + NORMAL_CLOCKWISE_WALL_KICKS[wall_kick_index + 1];
        }else{
            kicked_x = curr_piece_x + NORMAL_COUNTERCLOCKWISE_WALL_KICKS[wall_kick_index];
            kicked_y = curr_piece_y + NORMAL_COUNTERCLOCKWISE_WALL_KICKS[wall_kick_index + 1];
        }

        if(!check_collision(kicked_x, kicked_y)) {
            curr_piece_x = kicked_x;
            curr_piece_y = kicked_y;

            #define rotation_index_offset (direction == CLOCKWISE? 1 : 3)

            curr_rotation_index = (curr_rotation_index + rotation_index_offset) % 4;
            return;
        }
    }

    //wall kick failed, do not rotate
    for(i4 = 0; i4 < 8; i4++){
        curr_piece[i4] = old_piece[i4];
    }
}

UINT8 tetris_bag[7];
UINT8 future_bag[7];
UINT8 i5;
UINT8 t3;
UINT8 exchange_index;
void new_bag() {
    for(i5 = 0; i5 < 7; i5++) {
        tetris_bag[i5] = future_bag[i5];
    }

    for(i5 = 0; i5 < 7; i5++) {
        future_bag[i5] = i5;
    }

    for(i5 = 0; i5 < 7; i5++){
        t3 = future_bag[i5];
        exchange_index = i5 + (rand() % (7 - i5));
        future_bag[i5] = future_bag[exchange_index];
        future_bag[exchange_index] = t3;
    }
}

UINT8 curr_index = 0;
UINT8 get_next_piece() {
    if(curr_index == 7) {
        curr_index = 0;
        new_bag();
    }
    return tetris_bag[curr_index++];
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
UINT8 t4;
void swap_piece() {
    if(held_piece == 255) {
        held_piece = curr_piece_type;
        set_curr_piece(get_next_piece());
    }else{
        t4 = held_piece;
        held_piece = curr_piece_type;
        set_curr_piece(t4);
    }
}

#define PALETTE_1 *((UINT8*)0xFF49)

void tetris_init() {
    PALETTE_1 = 0x54;
    initrand(10);
    new_bag();
    new_bag();
    set_curr_piece(get_next_piece());
}

void tetris_show_board() {
    set_bkg_tiles(6, 0, 10, 18, board+220);
}

UINT8 has_collision;
UINT8 i1;
UINT8 cc_x, cc_y, cc_t;
UINT8 check_collision(UINT8 x, UINT8 y) {

    for(i1 = 0; i1 < 4; i1++){
        cc_x = x + curr_piece[2*i1];
        cc_y = y + curr_piece[2*i1 + 1];

        if(cc_x > 9 || cc_y > 40) {
            return 1;
        }
        
        if(tetris_board(cc_x, cc_y) != 0) {
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
    for(i = curr_piece_y; ; i--) {
        if(check_collision(curr_piece_x, i) != 0) {
            i++;
            break;
        }
    }
    
    new_x = curr_piece_x;
    new_y = i;
    
    for(i = 0; i < 4; i++){
        curr_pos_x = new_x + curr_piece[2*i];
        curr_pos_y = new_y + curr_piece[2*i + 1];

        if(curr_piece_type == I) {
            tetris_board(curr_pos_x, curr_pos_y) = I_sprites[4 * curr_rotation_index + i] + 1;
        }else{
            tetris_board(curr_pos_x, curr_pos_y) = curr_piece_type + 1;
        }
    }

    set_curr_piece(get_next_piece());

    clear_lines();

    tetris_show_board();
}

void show_curr_piece() {
    for(i = 0; i < 4; i++){
        if(curr_piece_type == I) {
            set_sprite_tile(i, I_sprites[4 * curr_rotation_index + i]);
        }else{
            set_sprite_tile(i, curr_piece_type);
        }
        
        curr_pos_x = curr_piece_x + curr_piece[2*i];
        curr_pos_y = curr_piece_y + curr_piece[2*i + 1];

        move_sprite(i, 56U + (curr_pos_x << 3), 152U - (curr_pos_y << 3));
    }
}

UINT8 x3, y3;
UINT8 i7;
UINT8 ghost_x, ghost_y;
void show_ghost_piece() {
    for(y3 = curr_piece_y; ; y3--){
        if(check_collision(curr_piece_x, y3) != 0) {
            y3++;
            break;
        }
    }

    for(i7 = 0; i7 < 4; i7++){
        set_sprite_tile(i7 + 4, 14);
        set_sprite_prop(i7 + 4, S_PALETTE);

        ghost_x = curr_piece_x + curr_piece[2*i7];
        ghost_y = y3 + curr_piece[2*i7 + 1];

        move_sprite(i7 + 4, 56U + (ghost_x << 3), 152U - (ghost_y << 3));
    }

}

#endif
