#ifndef MOVE_GEN_H_INCLUDED
#define MOVE_GEN_H_INCLUDED

#define MAXDEPTH 7
#define AB_PRUNE
#define BENCHMARK


#include <stdbool.h>

typedef struct{
    int start_pos;
    int end_pos;
    int special;
}MOVE;

#ifdef BENCHMARK
extern double count;
#endif

/* move_gen recursive function */
int move_gen( int *board, int depth, int alpha, int beta, bool black_or_white,
    int enpassant_square, bool long_castle_w, bool long_castle_b,
    bool short_castle_w, bool short_castle_b, MOVE *best_move);

/* change pos of a piece and change back */
void change_pos(int *board, int new_pos, int old_pos, int *copy_piece);
void change_pos_back(int *board, int new_pos, int old_pos, int copy_piece);

/* change pos for enpassant white and change backi */
void change_pos_enpassant_white(int *board, int new_pos, int old_pos, int *copy_piece);
void change_pos_back_enpassant_white(int *board, int new_pos, int old_pos,int copy_piece);

/*change pos for enpassant black and change back */
void change_pos_enpassant_black(int *board, int new_pos, int old_pos, int *copy_piece);
void change_pos_back_enpassant_black(int *board, int new_pos, int old_pos,int copy_piece);

#endif
