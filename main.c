#include <gb/gb.h>
#include <stdio.h>
#include "globals.h"
#include "piece_sprites.h"
#include "debug.h"
#include "tetris.h"

void update_switches() {
   HIDE_WIN;
   SHOW_SPRITES;
   SHOW_BKG;
}

void init() {
   DISPLAY_ON;
   set_bkg_data(0, 17, tile_data);
   set_sprite_data(0, 15, tile_data+16);
   t = 13;
   for(i = 0; i < 20; i++){
      for(j = 0; j < 18; j++){
         set_bkg_tiles(i, j, 1, 1, &t);
      }
   }

   tetris_init();
   tetris_show_board();
}

UINT8 rand_counter = 0;

UINT8 prev_up = 0;
UINT8 prev_a = 0;
UINT8 prev_b = 0;
UINT8 prev_select = 0;
UINT8 das_counter = 0;
UINT8 curr_key = 0;
UINT8 input = 0;
UINT8 move = 0;
void handle_input() {
   input = joypad();
   //handle DAS
   move = 0;
   if(curr_key) {
      if(input & curr_key) {
         das_counter++;
         if(das_counter > 9) move = 1;
      }else{
         curr_key = 0;
         das_counter = 0;
      }
   }else{
      initrand(rand_counter ^ rand()); //seed RNG with user input timing
      if(input & J_LEFT){
         curr_key = J_LEFT;
         move = 1;
      }else if(input & J_RIGHT) {
         curr_key = J_RIGHT;
         move = 1;
      }
   }

   if(input & J_DOWN && !check_collision(curr_piece_x, curr_piece_y - 1)) {
      curr_piece_y--;
   }

   if(move) {
      if(curr_key == J_LEFT && !check_collision(curr_piece_x - 1, curr_piece_y)) {
         curr_piece_x--;
      }
      if(curr_key == J_RIGHT && !check_collision(curr_piece_x + 1, curr_piece_y)) {
         curr_piece_x++;
      }
   }

   if(input & J_UP && !prev_up) {
      hard_drop();
      prev_up = 1;
   }
   if(!(input & J_UP)) {
      prev_up = 0;
   }

   if(input & J_A && !prev_a) {
      rotate(CLOCKWISE);
      prev_a = 1;
   }
   if(!(input & J_A)){
      prev_a = 0;
   }

   if(input & J_B && !prev_b) {
      rotate(COUNTERCW);
      prev_b = 1;
   }
   if(!(input & J_B)){
      prev_b = 0;
   }

   if(input & J_SELECT && !prev_select) {
      swap_piece();
      prev_select = 1;
   }
   if(!(input & J_SELECT)){
      prev_select = 0;
   }
}

void main() {
   printf("MODERN TETRIS\n\n");
   printf("PRESS START TO BEGIN\n");
   
   while(!(joypad() & J_START)){
      rand_counter++;
   }
   initrand(rand_counter);
   printf("%d\n", rand_counter);

   init();

   while(1) {
      rand_counter++;
      handle_input();
      handle_gravity();
      show_curr_piece();
      show_ghost_piece();
      update_switches();
      wait_vbl_done();
   }
}
