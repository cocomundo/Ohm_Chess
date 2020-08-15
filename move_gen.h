#ifndef MOVE_GEN_H_INCLUDED
#define MOVE_GEN_H_INCLUDED

//move_gen recursive function
int move_gen(int board[], int depth, int alpha, int beta, bool black_or_white );

//change pos of a piece and change back
void change_pos(int board[], int new_pos, int old_pos, int *copy_piece);
void change_pos_back(int board[], int new_pos, int old_pos, int copy_piece);

//piece functions
int white_R_Q_B(int board[], int pos, int dir[], int depth, int alpha, int beta);
int white_Kn(int board[], int pos, int dir[], int depth, int alpha, int beta);


//void white_P(Position position, int pos, int dir[], int depth);
//void white_Ki(Position position, bool long_c, bool short_c, int pos, int dir[], int depth);

int black_Kn(int board[], int pos, int dir[], int depth, int alpha, int beta);
int black_R_Q_B(int board[], int pos, int dir[], int depth, int alpha, int beta);

//void black_P(Position position, int pos, int dir[], int depth);
//void black_Ki(Position position, bool long_c, bool short_c, int pos, int dir[], int depth);
#endif
