#ifndef MOVE_GEN_H_INCLUDED
#define MOVE_GEN_H_INCLUDED

#define MAXDEPTH 8
#define HASHTABLE_ENTRY_DEPTH 2
#define AB_PRUNE
#define BENCHMARK
//#define HASHTABLE
#define HASHTABLE_SIZE 1048576


#include <stdbool.h>

typedef struct{
    int start_pos;
    int end_pos;
    int special;
}MOVE;

#ifdef BENCHMARK
extern double count;
extern double transpositiontable_hit_count;
#endif

#ifdef HASHTABLE
typedef struct{
    unsigned long long int hashkey;
    int depth;
    int eval;
}HASHTABLE_DATA;
#endif

/* move_gen recursive function */
int move_gen( int *board, int depth, int alpha, int beta, bool black_or_white,
    int enpassant_square, bool long_castle_w, bool long_castle_b,
    bool short_castle_w, bool short_castle_b, MOVE *best_move, int material, long long unsigned int hashkey_pos);

/* change pos of a piece and change back */
void change_pos(int *board, int new_pos, int old_pos, int *copy_piece, long long unsigned int *hashkey_next_pos);
void change_pos_back(int *board, int new_pos, int old_pos, int copy_piece);

/* change pos for enpassant white and change backi */
void change_pos_enpassant_white(int *board, int new_pos, int old_pos, int *copy_piece, long long unsigned int *hashkey_next_pos);
void change_pos_back_enpassant_white(int *board, int new_pos, int old_pos,int copy_piece);

/*change pos for enpassant black and change back */
void change_pos_enpassant_black(int *board, int new_pos, int old_pos, int *copy_piece, long long unsigned int *hashkey_next_pos);
void change_pos_back_enpassant_black(int *board, int new_pos, int old_pos,int copy_piece);

#endif
