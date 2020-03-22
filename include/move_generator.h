#ifndef MOVE_GENERATOR_H_INCLUDED
#define MOVE_GENERATOR_H_INCLUDED

#include "position.h"
#define MAX_FIELD 120
void gamestart(bool *p_to_move, int board[]);
void move_gen(Position position);
//void test_move(int board[],int pos,int new_pos);

void white_P(int board[], int pos, int dir[]);
void white_Kn(int board[], int pos, int dir[]);
void white_R_Q_B(int board[],int pos, int dir[]);
void pos_eval(int board[]);
void white_Ki(int board[], int pos, int dir[]);


/*
weisserbauer(i);
weisserspringer(i);

weisserdame(i);
weisserkoenig(i);
*/
#endif