#include "tetris.h"
#include "scoring.h"
#include "text.h"

UINT8 board[400];

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
UINT8 light_gray_bkg[100];

UINT8 I_sprites[16] = {
    9, 10, 10, 11,
    6, 7, 7, 8,
    11, 10, 10, 9,
    8, 7, 7, 6
};

UINT8 piece_previews[56] = {
    16, 16, 1, 16, 16, 1, 1, 1,
    16, 2, 2, 16, 16, 2, 2, 16,
    16, 3, 16, 16, 16, 3, 3, 3,
    16, 16, 16, 4, 16, 4, 4, 4,
    16, 5, 5, 16, 16, 16, 5, 5,
    16, 16, 6, 6, 16, 6, 6, 16,
    16, 16, 16, 16, 10, 11, 11, 12,
};

INT8 NORMAL_CLOCKWISE_WALL_KICKS[40] = {
    0, 0, -1, 0, -1, 1, 0, -2, -1, -2,
    0, 0, 1, 0, 1, -1, 0, 2, 1, 2,
    0, 0, 1, 0, 1, 1, 0, -2, 1, -2,
    0, 0, -1, 0, -1, -1, 0, 2, -1, -2
};

INT8 NORMAL_COUNTERCLOCKWISE_WALL_KICKS[40] = {
    0, 0, 1, 0, 1, 1, 0, -2, 1, -2,
    0, 0, 1, 0, 1, -1, 0, 2, 1, 2,
    0, 0, -1, 0, -1, 1, 0, -2, -1, -2,
    0, 0, -1, 0, -1, -1, 0, 2, -1, 2
};

INT8 I_CLOCKWISE_WALL_KICKS[40] = {
    0, 0, -2, 0, 1, 0, -2, -1, 1, 2,
    0, 0, -1, 0, 2, 0, -1, 2, 2, -1,
    0, 0, 2, 0, -1, 0, 2, 1, -1, -2,
    0, 0, 1, 0, -2, 0, 1, -2, -2, 1
};

INT8 I_COUNTERCLOCKWISE_WALL_KICKS[40] = {
    0, 0, -1, 0, 2, 0, -1, 2, 2, -1,
    0, 0, 2, 0, -1, 0, 2, 1, -1, -2,
    0, 0, 1, 0, -2, 0, 1, -2, -2, 1,
    0, 0, -2, 0, 1, 0, -2, -1, 1, 2
};

UINT8 GRAVITY_FRAMES[15] = {60, 47, 37, 28, 21, 16, 11, 8, 6, 4, 3, 2, 1, 1, 1};
UINT8 GRAVITY_LINES[15] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3};
UINT8 gravity_counter = 0;

UINT8 LOCK_DELAYS[] = {27, 25, 23, 21, 20, 19, 18, 17, 16, 15};
#define LOCK_DELAY_MAX (curr_level <= 15? 30 : (curr_level < 25? LOCK_DELAYS[curr_level - 15] : 15))

UINT8 bounding_box_size[7] = {3, 2, 3, 3, 3, 3, 4}; //width and height

INT8 curr_piece_x, curr_piece_y;
UINT8 curr_piece_type;

UINT8 curr_piece[8];
UINT8 curr_rotation_index = 0;
UINT8 curr_level = 1;

UINT8 i2;
void copy_piece(UINT8 *src, UINT8 *dest) {
    for(i2 = 0; i2 < 8; i2++){
        dest[i2] = src[i2];
    }
}

void game_over() {
    write_text(" GAME OVER", 6, 8);
    while(1);
}

void set_curr_piece(UINT8 type) {
    copy_piece(pieces + 8*type, curr_piece);
    curr_piece_type = type;
    curr_rotation_index = 0;
    curr_piece_x = 5 - (bounding_box_size[curr_piece_type] + 1) / 2;
    curr_piece_y = (curr_piece_type == 1? 16: 15);

    if(check_collision(curr_piece_x, curr_piece_y)) {
        game_over();
    }

    lock_delay_counter = LOCK_DELAY_MAX;
    lock_delay_active = 0;
    lock_delay_extensions = lock_delay_extensions_max;
    lock_delay_extensions_min_y = curr_piece_y;

    gravity_counter = 0;
}

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

        //need repetition b/c dynamic arrays are gimmicky in gbdk
        if(curr_piece_type == I) {
            if(direction == CLOCKWISE){
                kicked_x = curr_piece_x + I_CLOCKWISE_WALL_KICKS[wall_kick_index];
                kicked_y = curr_piece_y + I_CLOCKWISE_WALL_KICKS[wall_kick_index + 1];
            }else{
                kicked_x = curr_piece_x + I_COUNTERCLOCKWISE_WALL_KICKS[wall_kick_index];
                kicked_y = curr_piece_y + I_COUNTERCLOCKWISE_WALL_KICKS[wall_kick_index + 1];
            }
        }else{
            if(direction == CLOCKWISE){
                kicked_x = curr_piece_x + NORMAL_CLOCKWISE_WALL_KICKS[wall_kick_index];
                kicked_y = curr_piece_y + NORMAL_CLOCKWISE_WALL_KICKS[wall_kick_index + 1];
            }else{
                kicked_x = curr_piece_x + NORMAL_COUNTERCLOCKWISE_WALL_KICKS[wall_kick_index];
                kicked_y = curr_piece_y + NORMAL_COUNTERCLOCKWISE_WALL_KICKS[wall_kick_index + 1];
            }
        }

        if(!check_collision(kicked_x, kicked_y)) { //successful rotation
            if(lock_delay_active && lock_delay_extensions != 0) {
                lock_delay_counter = LOCK_DELAY_MAX;
                lock_delay_extensions--;
            }

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
        exchange_index = i5 + ((UINT8)rand() % (7 - i5));
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

#define next_piece(n) (n + curr_index < 7? tetris_bag[n + curr_index] : future_bag[n + curr_index - 7])

UINT8 i8;
void show_piece_previews() {
    set_bkg_tiles(16, 0, 4, 16, light_gray_bkg);
    for(i8 = 0; i8 < 5; i8++) { 
        set_bkg_tiles(16, 3*i8 + 1, 4, 2, piece_previews + 8 * next_piece(i8));
    }
}

UINT8 x2, y2, i6, t5;
UINT8 has_hole;
UINT8 curr_line_to_copy;
UINT8 lines_to_copy[40];
UINT8 *row;
UINT8 *row1;
UINT8 lines_cleared;
void clear_lines() {
    curr_line_to_copy = 0;
    lines_cleared = 0;
    for(y2 = 0; y2 != 40; y2++) {
        has_hole = 0;
        row = tetris_row(y2);
        for(x2 = 0; x2 != 10; x2++){
            if(row[x2] == 0) {
                has_hole = 1;
            }
        }

        if(has_hole) {
            lines_to_copy[curr_line_to_copy] = y2;
            curr_line_to_copy++;
        }else{
            lines_cleared++;
        }
    }
    
    if(lines_cleared != 0) {
        count_lines(lines_cleared);
    }

    if(curr_line_to_copy == 40) return;

    for(y2 = curr_line_to_copy; y2 != 40; y2++) {
        row = tetris_row(y2);
        for(x2 = 0; x2 != 0; x2++) {
            row[x2] = 0;
        }
    }

    for(y2 = 0; y2 != curr_line_to_copy; y2++){
        t5 = lines_to_copy[y2];
        row = tetris_row(y2);
        row1 = tetris_row(t5);
        for(x2 = 0; x2 != 10; x2++) {
            row[x2] = row1[x2];
        }
    }
}

UINT8 held_piece = 255;
UINT8 t4;

void show_held_piece() {
    set_bkg_tiles(2, 0, 4, 4, light_gray_bkg);
    if(held_piece != 255) {
        set_bkg_tiles(2, 1, 4, 2, piece_previews + 8 * held_piece);
    }
}

UINT8 can_swap = 1;
void swap_piece() {
    if(!can_swap) {
        return;
    }
    if(held_piece == 255) {
        held_piece = curr_piece_type;
        set_curr_piece(get_next_piece());
        show_piece_previews();
    }else{
        t4 = held_piece;
        held_piece = curr_piece_type;
        set_curr_piece(t4);
    }

    can_swap = 0;

    show_held_piece();
}

#define PALETTE_1 *((UINT8*)0xFF49)

UINT8 i9;
void tetris_init() {
    PALETTE_1 = 0x54;
    for(i9 = 0; i9 < 100; i9++) {
        light_gray_bkg[i9] = 16;
    }

    new_bag();
    new_bag();
    set_curr_piece(get_next_piece());
    show_piece_previews();
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
static UINT8 i;
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

    can_swap = 1;

    set_curr_piece(get_next_piece());
    show_piece_previews();

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

UINT8 move_piece(INT8 direction) {
    if(direction == DOWN){
        if(!check_collision(curr_piece_x, curr_piece_y - 1)){
            curr_piece_y--;
            if(curr_piece_y < lock_delay_extensions_min_y) {
                lock_delay_extensions = lock_delay_extensions_max;
                lock_delay_extensions_min_y = curr_piece_y;
            }
            return 1;
        }else{
            lock_delay_active = 1;
            return 0;
        }
    }else{
        if(!check_collision(curr_piece_x + direction, curr_piece_y)) {
            curr_piece_x += direction;
            if(lock_delay_active && lock_delay_extensions != 0) {
                lock_delay_counter = LOCK_DELAY_MAX;
                lock_delay_extensions--;
            }
            return 1;
        }else{
            return 0;
        }
    }
}

UINT8 lock_delay_active = 0;
UINT8 lock_delay_counter = 30;
UINT8 lock_delay_extensions_max = 15;
UINT8 lock_delay_extensions = 15;
INT8 lock_delay_extensions_min_y = 20;
void handle_lock_delay() {
    if(lock_delay_active) {
        lock_delay_counter--;
        if(lock_delay_counter == 0) {
            hard_drop();

            lock_delay_active = 0;
            lock_delay_counter = LOCK_DELAY_MAX;
        }
    }
}

#define frames_to_wait (curr_level > 15? 1 : GRAVITY_FRAMES[curr_level-1])
UINT8 lines_to_drop = 0;
UINT8 i10 = 0;
void handle_gravity() { //called every frame
    gravity_counter++;
    if(gravity_counter >= frames_to_wait) {
        gravity_counter = 0;
        
        lines_to_drop = (curr_level > 15? 18 : GRAVITY_LINES[curr_level-1]);
        for(i10 = 0; i10 < lines_to_drop; i10++) {
            if(!move_piece(DOWN)) break;
        }
    }
}