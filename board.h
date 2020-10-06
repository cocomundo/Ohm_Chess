#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "move_gen.h"
#include <stddef.h>
#include <stdbool.h>
/*chessboard*/
extern int board[120];

extern int long_castle_white;
extern int long_castle_black;
extern int short_castle_white;
extern int short_castle_black;
extern int move_50_rule;

int lega_move_check_player();
void make_move(int start_pos, int end_pos, int special_move,
    int *move_rule, bool *w_b, int *game_state);

int mate(bool color_to_move);
int insufficient_material();
/*
* 1 == enpassant
* 2 == long_castle_w
* 3 == long_castle_b
* 4 == short_castle_w
* 5 == short_castle_b
* 6 == prom_kn
* 7 == prom_b
* 8 == prom_r
* 9 == prom_q
*/
#endif
