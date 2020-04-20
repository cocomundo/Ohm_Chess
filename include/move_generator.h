/*#ifndef MOVE_GENERATOR_H_INCLUDED
#define MOVE_GENERATOR_H_INCLUDED

#include "position.h"
#include "Direction.h"
#define MAX_FIELD 120

void gamestart(bool *p_to_move, int board[]);
void move_gen(Position position, int depth);
//void test_move(int board[],int pos,int new_pos);

bool chess_check(Position position, bool move_b_w);
void white_P(Position position, int pos, int dir[], int depth);
void white_Kn(Position position, int pos, int dir[], int depth);
void white_R_Q_B(Position position,int pos, int dir[], int depth);
void white_Ki(Position position, bool long_c, bool short_c, int pos, int dir[], int depth);

void black_P(Position position, int pos, int dir[], int depth);
void black_Kn(Position position, int pos, int dir[], int depth);
void black_R_Q_B(Position position, int pos, int dir[], int depth);
void black_Ki(Position position, bool long_c, bool short_c, int pos, int dir[], int depth);

int pos_eval(int board[]);


/*
weisserbauer(i);
weisserspringer(i);

weisserdame(i);
weisserkoenig(i);

#endif
*/
