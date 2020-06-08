#include <gb/gb.h>
#include <stdio.h>
#include "globals.h"
#include "piece_sprites.h"
#include "tetris.h"

void updateSwitches() {
   HIDE_WIN;
   SHOW_SPRITES;
   SHOW_BKG;
}

void init() {
   tetris_init();
   DISPLAY_ON;
   set_bkg_data(0, 16, tetris_pieces_tile_data);
   set_sprite_data(0, 15, tetris_pieces_tile_data+16);
   t = 13;
   for(i = 0; i < 20; i++){
      for(j = 0; j < 18; j++){
         set_bkg_tiles(i, j, 1, 1, &t);
      }
   }
   tetris_show_board();
}

UINT8 prev_up = 0;
UINT8 prev_a = 0;
UINT8 das_counter = 0;
UINT8 current_key = 0;
UINT8 input = 0;
UINT8 move = 0;
void handle_input() {
   input = joypad();
   if(input & J_UP && !prev_up) {
      hard_drop();
      prev_up = 1;
   }
   if(!(input & J_UP)) {
      prev_up = 0;
   }

   if(input & J_A && !prev_a) {
      rotate_clockwise();
      prev_a = 1;
   }
   if(!(input & J_A)){
      prev_a = 0;
   }
   
   move = 0;
   if(current_key) {
      if(input & current_key) {
         das_counter++;
         if(das_counter > 9) move = 1;
      }else{
         current_key = 0;
         das_counter = 0;
      }
   }else{
      if(input & J_DOWN) {
         current_key = J_DOWN;
         move = 1;
      }else if(input & J_LEFT){
         current_key = J_LEFT;
         move = 1;
      }else if(input & J_RIGHT) {
         current_key = J_RIGHT;
         move = 1;
      }
   }

   if(move) {
      if(current_key == J_DOWN && !check_collision(current_piece_position[0], current_piece_position[1] - 1)) {
         current_piece_position[1]--;
      }
      if(current_key == J_LEFT && !check_collision(current_piece_position[0] - 1, current_piece_position[1])) {
         current_piece_position[0]--;
      }
      if(current_key == J_RIGHT && !check_collision(current_piece_position[0] + 1, current_piece_position[1])) {
         current_piece_position[0]++;
      }
   }
}

void main() {
   init();

   while(1) {
      handle_input();
      tetris_show_current_piece();
      updateSwitches();
      wait_vbl_done();
   }
}
