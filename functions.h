#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#define MAX_FIELD 120
void gamestart(bool *p_to_move, int board[]);
void move_f(bool *p_to_move, int board[]);

void white_P(int board[], int pos, int dir[]);
void white_Kn_Ki(int board[], int pos, int dir[]);
void white_R_Q_B(int board[],int pos, int dir[]);
void pos_eval(int board[]);

/*
weisserbauer(i);
weisserspringer(i);

weisserdame(i);
weisserkoenig(i);
*/
#endif
