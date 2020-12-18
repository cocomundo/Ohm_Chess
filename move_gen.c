#include <stdio.h>
#include <limits.h> /* INT_MIN, INT_MAX */
#include "board.h"

#include "move_gen.h"
#include "evaluation.h"


/* directions, first number of array represents the amount of directions */
int dir_B[5]={4,-9,9,-11,11};
int dir_R[5]={4,-1,1,-10,10};
int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
int dir_wP[5]={4,-10,-20,-9,-11};
int dir_bP[5]={4,10,20,9,11};

int material_list[17]={0,100,280,300,500,900,100000,0,0,0,0,-100,-280,-300,-500,-900,-100000};
int material=0;

#ifdef HASHTABLE
HASHTABLE_DATA transpositontable_white[HASHTABLE_SIZE]={0};
HASHTABLE_DATA transpositontable_black[HASHTABLE_SIZE]={0};
#endif

double count = 0;
double transpositiontable_hit_count =0;
/* Explanation of the move_gen agruments:
* int *board: Pointer to the chess board
* int depth: How deep the searchtree is searching
* int alpha, int beta: Used for alpha-beta-pruning
* bool black_or_white: White is next to move==1 black is next to move==0
* int enpassant_square: If a pawn moves 2 squares, the square is stored in this variable for 1 move to give the opponent the chance to capture it
* bool ...castle: 1 means castles is allowed 0 means it is not
* MOVE *best_move: Pointer to where the best move in the position is stored
* int material: The count of the Material on the board, white pieces have positive and black pieces have nevative values
* unsigned long long int hashkey_pos: The hashkey of the current position
*/
int move_gen( int *board, int depth, int alpha, int beta, bool black_or_white,
    int enpassant_square, bool long_castle_w, bool long_castle_b,
    bool short_castle_w, bool short_castle_b, MOVE *best_move, int material, unsigned long long int hashkey_pos)
{ 
    int eval = 0; /* stores the evaluation value of the calculated move */
    int copy_piece = 0; /* store the captured piece */
    int copy_pos = 0; /* store the previous square */
    unsigned long long int hashkey_next_pos=0;

/* count the Material on the board at the beginning of the recursiv search*/
    if (depth== MAXDEPTH)
    {
        material=0;
        for(int i = 20; i < 100; i++) {
            /*Counts the Material on the Board, with the values of the material list*/
            if(board[i]>0)
            {
                material+=material_list[board[i]];
            }
            
        }
    }
    if ( depth == 0 ) {
        #ifdef BENCHMARK
        count += 1;
        #endif
        return evaluation(board, material);
    }
    #ifdef HASHTABLE
    /*Look up the hashkey in the hashtable*/
    if(black_or_white)
    {
        if(transpositontable_white[hashkey_pos%HASHTABLE_SIZE].hashkey==hashkey_pos && transpositontable_white[hashkey_pos%HASHTABLE_SIZE].depth>=depth)
        {
            transpositiontable_hit_count++;
            return transpositontable_white[hashkey_pos%HASHTABLE_SIZE].eval;
        }
    } else
    {
        if(transpositontable_black[hashkey_pos%HASHTABLE_SIZE].hashkey==hashkey_pos && transpositontable_black[hashkey_pos%HASHTABLE_SIZE].depth>=depth)
        {
            transpositiontable_hit_count++;
            return transpositontable_black[hashkey_pos%HASHTABLE_SIZE].eval;
        }
    }
    
    #endif

    /* white*/
    if(black_or_white) { 
        int max = INT_MIN;

        for (int square = 20; square < 100; square++){
            switch (board[square]) {
                case 1: /* Pawn */
                    if(board[square+dir_wP[1]] == 0) {
                        if(square+dir_wP[1] < 30) {
                            /* promotion */
                            board[square]=0;

                            /* convert the pawn into another piece */
                            for(int i = 2; i < 6; i++)
                            {
                                board[square+dir_wP[1]] = i;
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                hashkey_next_pos=hashkey_next_pos^random_number[1*square];
                                hashkey_next_pos=hashkey_next_pos^random_number[i*(square+dir_wP[1])];
                                #endif
                                
                                /* after white move -> black move -> false */
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL,material+material_list[i]-material_list[1], hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                
                                #endif
                                if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_wP[1];
                                    best_move->special = i+4;
                                }
                                max = (eval > max ? eval : max);
                            }
                            /*change Position back*/
                            board[square]=1;
                            board[square+dir_wP[1]]=0;

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif

                        }
                        if(square > 80 && (board[square+dir_wP[2]] == 0)) {
                            
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            /* move two squares, if still on 2nd rank */
                            change_pos(board, square+dir_wP[2], square, &copy_piece, &hashkey_next_pos);

                            /* after white move -> black move -> false */
                            eval = move_gen(board, depth-1, alpha, beta, false, square+dir_wP[2], long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                            #endif
                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = square+dir_wP[2];
                                best_move->special = 0;
                            }
                            max = (eval > max ? eval : max);

                            change_pos_back(board, square+dir_wP[2], square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif
                        }
                        #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                        /* move one square */
                        change_pos(board, square+dir_wP[1], square, &copy_piece, &hashkey_next_pos);

                        /* after white move -> black move -> false */
                        eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                        #ifdef HASHTABLE
                        /*enter data in the hashtable*/
                        if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                        #endif
                        if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = square+dir_wP[1];
                            best_move->special = 0;
                        }
                        max = (eval > max ? eval : max);

                        change_pos_back(board, square+dir_wP[1], square, copy_piece);

                        #ifdef AB_PRUNE
                        alpha = (alpha > eval ? alpha : eval);
                        if (beta <= alpha)
                            return max;
                        #endif
                    }
                    /*White Pawn Capture*/
                    for(int y = 3; y < 5; y++) {
                        if(board[square+dir_wP[y]] > 10) {
                            /*capture into promotion */
                            if(square+dir_wP[y] < 30) {
                                /* If King is captured game is over*/
                                if(board[square+dir_wP[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);

                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                board[square]=0;
                                copy_piece=board[square+dir_wP[y]];
                                /* convert the pawn into another piece */
                                for(int i = 2; i < 6; i++)
                                {
                                    board[square+dir_wP[y]] = i;
                                    #ifdef HASHTABLE
                                    hashkey_next_pos=hashkey_pos;
                                    hashkey_next_pos=hashkey_next_pos^random_number[1*square];
                                    hashkey_next_pos=hashkey_next_pos^random_number[i*square+dir_wP[y]];
                                    #endif
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL,material+material_list[i]-material_list[1]-material_list[copy_piece], hashkey_next_pos);
                                    #ifdef HASHTABLE
                                    /*enter data in the hashtable*/
                                    if(depth>=HASHTABLE_ENTRY_DEPTH)
                                    {
                                        transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                        transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                        transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                    }
                                    #endif
                                    if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = square+dir_wP[y];
                                        best_move->special = 4+i;
                                    }
                                    max = (eval > max ? eval : max);
                                }
                                /*change Position back*/
                                board[square]=1;
                                board[square+dir_wP[y]]=copy_piece;


                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif
                            }else {
                                /* If King is captured game is over*/
                                if(board[square+dir_wP[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);

                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                /* capture */
                                change_pos(board, square+dir_wP[y], square, &copy_piece, &hashkey_next_pos);

                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_wP[y];
                                    best_move->special = 0;
                                }
                                max = (eval > max ? eval : max);

                                change_pos_back(board, square+dir_wP[y], square, copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif
                            }

                        }
                    }
                    if( (enpassant_square - square) == 1 ||
                        (enpassant_square - square) == -1 ){
                        #ifdef HASHTABLE
                        hashkey_next_pos=hashkey_pos;
                        #endif
                        /* en passant */
                        change_pos_enpassant_white(board, enpassant_square, square, &copy_piece, &hashkey_next_pos);
                        eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[11], hashkey_next_pos);
                        #ifdef HASHTABLE
                        /*enter data in the hashtable*/
                        if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                        #endif
                        if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = enpassant_square - 10;
                            best_move->special = 1;
                        }
                        max = (eval > max ? eval : max);

                        change_pos_back_enpassant_white(board, enpassant_square, square, copy_piece );

                        #ifdef AB_PRUNE
                        alpha = (alpha > eval ? alpha : eval);
                        if (beta <= alpha)
                            return max;
                        #endif
                    }
                    break;
                case 2: /* Knight */
                    for(int y = 1; y <= dir_Kn[0]; y++) {
                        /* move or capture */
                        if(board[square+dir_Kn[y]] == 0 || board[square+dir_Kn[y]] > 10) {
                            /* If King is captured game is over*/
                            if(board[square+dir_Kn[y]] == 16)
                                return ((INT_MAX - MAXDEPTH)+ depth);

                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            change_pos(board, square+dir_Kn[y], square, &copy_piece, &hashkey_next_pos);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif
                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = square+dir_Kn[y];
                                best_move->special = 0;
                            }
                            max = (eval > max ? eval : max);

                            change_pos_back(board, square+dir_Kn[y], square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif
                        }
                    }
                    break;
                case 3: /* Bishop */
                    copy_pos = square; 
                    /* check every direction the Bishop can move */
                    for(int y = 1; y <= dir_B[0]; y++) { 
                        while(board[copy_pos+dir_B[y]] == 0 || board[copy_pos+dir_B[y]] > 10) {
                            /* capture */
                            if (board[copy_pos+dir_B[y]] > 10) {
                                /* If King is captured game is over*/
                                if(board[copy_pos+dir_B[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);

                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece, &hashkey_next_pos);

                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_B[y];
                                    best_move->special = 0;
                                }
                                max = (eval > max ? eval : max);

                                change_pos_back(board, (copy_pos+dir_B[y]), square, copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif

                                /* Stop, because a piece is captured */
                                break; 
                            } 
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            /* move */
                            change_pos(board, copy_pos+dir_B[y], square, &copy_piece, &hashkey_next_pos);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif
                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_B[y];
                                best_move->special = 0;
                            }
                            max = (eval > max ? eval : max);

                            change_pos_back(board, copy_pos+dir_B[y], square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif

                            copy_pos += dir_B[y];
                        }
                        /* after each direction is done, the copy pos needs to be resettet */
                        copy_pos = square;
                    }
                    break;
                case 4: /* Rook */
                    copy_pos = square; 
                    /* check every direction the piece can go */
                    for(int y = 1; y <= dir_R[0]; y++) {
                        while(board[copy_pos+dir_R[y]] == 0 || board[copy_pos+dir_R[y]] > 10) {
                            /* capture */
                            if (board[copy_pos+dir_R[y]] > 10) {

                                /* If King is captured game is over*/
                                if(board[copy_pos+dir_R[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);
                                
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece, &hashkey_next_pos);

                                /* If the Rook moves from it's starting square, castles to this side becomes illegal*/
                                if(long_castle_w ==1 && square == 91){
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                }else if(short_castle_w==1 && square == 98){
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, false, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                }else{
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                }
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif 

                                if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_R[y];
                                    best_move->special = 0;
                                }
                                max = (eval > max ? eval : max);

                                change_pos_back(board, (copy_pos+dir_R[y]),square , copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif

                                /* Stop, because a piece is captured */
                                break; 
                            }
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            /* move */
                            change_pos(board, copy_pos+dir_R[y], square, &copy_piece, &hashkey_next_pos);
                            
                            /* If the Rook moves from it's starting square, castles to this side becomes illegal*/
                            if(square == 91){
                                eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                            }else if(square == 98){
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, false, short_castle_b, NULL, material, hashkey_next_pos);
                            }else{
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                            }
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif 

                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_R[y];
                                best_move->special = 0;
                            }
                            max = (eval > max ? eval : max);

                            change_pos_back(board, copy_pos+dir_R[y], square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif

                            copy_pos += dir_R[y];
                        }
                        /*after each direction is done, the copy pos needs to be resettet */
                        copy_pos=square;
                    }
                    break;
                case 5: /* Queen */
                    copy_pos = square; 
                    /* check every direction the piece can go */
                    for(int y = 1; y <= dir_Q_Ki[0]; y++) {
                        while(board[copy_pos+dir_Q_Ki[y]] == 0 || board[copy_pos+dir_Q_Ki[y]] > 10) {
                            /* capture */
                            if (board[copy_pos+dir_Q_Ki[y]] > 10) {
                                /* If King is captured game is over*/
                                if(board[copy_pos+dir_Q_Ki[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);
                                
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece, &hashkey_next_pos);
                                /* after white move -> black move */
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                    best_move->special = 0;
                                }
                                max = (eval > max ? eval : max);

                                change_pos_back(board, (copy_pos+dir_Q_Ki[y]),square , copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif

                                /* Stop, because a piece is captured */
                                break; 
                            }
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            /* move */
                            change_pos(board, copy_pos+dir_Q_Ki[y], square, &copy_piece, &hashkey_next_pos);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif
                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                best_move->special = 0;
                            }
                            max = (eval > max ? eval : max);

                            change_pos_back(board, copy_pos+dir_Q_Ki[y], square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif

                            copy_pos += dir_Q_Ki[y];
                        }
                        /* after each direction is done, the copy pos has to be reset */
                        copy_pos = square;
                    }
                    break;
                case 6:/* King */
                    for(int y = 1; y < dir_Q_Ki[0]; y++) {
                        if(board[square+dir_Q_Ki[y]] == 0 || board[square+dir_Q_Ki[y]] > 10) {
                            /* If King is captured game is over*/
                            if(board[square+dir_Q_Ki[y]] == 16)
                                return ((INT_MAX - MAXDEPTH)+ depth);

                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            change_pos(board,square+dir_Q_Ki[y],square, &copy_piece, &hashkey_next_pos);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, false, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif
                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                best_move->special = 0;
                            }
                            max = (eval > max ? eval : max);

                            change_pos_back(board,square+dir_Q_Ki[y],square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif

                        }
                    }
                    if(long_castle_w==1 && square==95 && board[94]==0 && board[93]==0 && board[92]==0 && board[91]==4)
                    {
                        /*testing if the King is in check or moves thew check, by giving the opponent the move*/
                        int in_check=0;
                        
                        in_check=move_gen(board, 1, INT_MIN, INT_MAX, true, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                        board[95]=0;
                        board[94]=6;
                        in_check|=move_gen(board, 1, INT_MIN, INT_MAX, true, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                        board[95]=6;
                        board[94]=0;
                        if(in_check>-50000)
                        {
                            board[91]=0;
                            board[93]=6;
                            board[94]=4;
                            board[95]=0;
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            hashkey_next_pos=hashkey_next_pos^random_number[6*93];
                            hashkey_next_pos=hashkey_next_pos^random_number[6*95];
                            hashkey_next_pos=hashkey_next_pos^random_number[4*91];
                            hashkey_next_pos=hashkey_next_pos^random_number[4*94];
                            #endif 
                            eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, false, short_castle_b, NULL, material, hashkey_next_pos);
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif
                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = 0;
                                best_move->end_pos = 0;
                                best_move->special = 4;
                            }
                            max = (eval > max ? eval : max);

                            board[91]=4;
                            board[93]=0;
                            board[94]=0;
                            board[95]=6;

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif
                        }
                    }
                    if(short_castle_w==1 &&square==95 && board[96]==0 && board[97]==0 && board[98]==4)
                    {
                        /*testing if the King is in check or moves thew check, by giving the opponent the move*/
                        int in_check=0;
                        
                        in_check=move_gen(board, 1, INT_MIN, INT_MAX, true, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                        board[95]=0;
                        board[96]=6;
                        in_check|=move_gen(board, 1, INT_MIN, INT_MAX, true, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                        board[95]=6;
                        board[96]=0;

                        if(in_check>-50000)
                        {

                            board[95]=0;
                            board[96]=4;
                            board[97]=6;
                            board[98]=0;
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            hashkey_next_pos=hashkey_next_pos^random_number[6*97];
                            hashkey_next_pos=hashkey_next_pos^random_number[6*95];
                            hashkey_next_pos=hashkey_next_pos^random_number[4*98];
                            hashkey_next_pos=hashkey_next_pos^random_number[4*96];
                            #endif
                            eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, false, short_castle_b, NULL, material, hashkey_next_pos);
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].depth=MAXDEPTH-depth;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_white[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif
                            if((depth == MAXDEPTH) && (eval > max) && (best_move != NULL)){
                                best_move->start_pos = 0;
                                best_move->end_pos = 0;
                                best_move->special = 2;
                            }
                            max = (eval > max ? eval : max);

                            board[95]=6;
                            board[96]=0;
                            board[97]=0;
                            board[98]=4;

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif
                        }

                    }
                    break;

                default:break;
            }
        }
        return max;

    } else {
        /* black */
        int min = INT_MAX;        

        for (int square = 20; square < 100; square++) {
            switch (board[square]) {
                case 11:/* Pawn */
                    if(board[square+dir_bP[1]] == 0) {
                        if(square+dir_bP[1] > 90) {
                            /* promotion */
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            board[square]=0;
                            for(int i = 12; i < 16; i++)
                            {
                                board[square+dir_bP[1]] = i;
                                #ifdef HASHTABLE
                                #ifdef HASHTABLE
                                    hashkey_next_pos=hashkey_pos;
                                    hashkey_next_pos=hashkey_next_pos^random_number[1*square];
                                    hashkey_next_pos=hashkey_next_pos^random_number[i*square+dir_bP[1]];
                                    #endif
                            
                                #endif
                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[11]+material_list[i], hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_bP[1];
                                    best_move->special = i-6;
                                }
                                min = (eval < min ? eval : min);
                            }
                            /*change Position back*/
                            board[square]=11;
                            board[square+dir_bP[1]]=0;

                            #ifdef AB_PRUNE
                            beta = (beta < eval ? beta : eval);
                            if (beta <= alpha)
                                return min;
                            #endif

                        }
                        if((square < 40) && (board[square+dir_bP[2]] == 0)) {
                            #ifdef HASHTABLE
                            hashkey_next_pos=hashkey_pos;
                            #endif
                            /* move two squares, if still on 2nd rang */
                            change_pos(board, square+dir_bP[2], square, &copy_piece, &hashkey_next_pos);

                            /* after black move -> white move -> true */
                            eval = move_gen(board, depth-1, alpha, beta, true, square+dir_bP[2], long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos); 
                            #ifdef HASHTABLE
                            /*enter data in the hashtable*/
                            if(depth>=HASHTABLE_ENTRY_DEPTH)
                            {
                                transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                            }
                            #endif
                            if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = square+dir_bP[2];
                                best_move->special = 0;
                            }
                            min = (eval < min ? eval : min);

                            change_pos_back(board, square+dir_bP[2], square, copy_piece);

                            #ifdef AB_PRUNE
                            beta = (beta < eval ? beta : eval);
                            if (beta <= alpha)
                                return min;
                            #endif
                        }
                        #ifdef HASHTABLE
                        hashkey_next_pos=hashkey_pos;
                        #endif
                        /* move one square */
                        change_pos(board, square+dir_bP[1], square, &copy_piece, &hashkey_next_pos);

                        /* after black move -> white move -> true */
                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                        if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = square+dir_bP[1];
                            best_move->special = 0;
                        }
                        min = (eval < min ? eval : min);

                        change_pos_back(board, square+dir_bP[1], square, copy_piece);

                        #ifdef AB_PRUNE
                        beta = (beta < eval ? beta : eval);
                        if (beta <= alpha)
                            return min;
                        #endif

                    }
                    for(int y = 3; y < 5; y++) {
                        if(board[square+dir_bP[y]] < 10 && board[square+dir_bP[y]] > 0) {
                            if(square+dir_bP[y] > 90) {
                                /* If King is captured game is over*/
                                if(board[square+dir_bP[y]] == 6)
                                    return ((INT_MIN + MAXDEPTH) - depth);

                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                /* capture into promotion */

                                board[square]=0;
                                copy_piece=board[square+dir_bP[y]];
                                /* convert the pawn into another piece */
                                for(int i = 12; i < 16; i++)
                                {
                                    board[square+dir_bP[y]] = i;
                                    #ifdef HASHTABLE
                                    hashkey_next_pos=hashkey_pos;
                                    hashkey_next_pos=hashkey_next_pos^random_number[11*square];
                                    hashkey_next_pos=hashkey_next_pos^random_number[i*square+dir_bP[y]];
                                    #endif
                                    /* after black move -> white move -> true */
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material+material_list[i]-material_list[11]-material_list[copy_piece], hashkey_next_pos); 
                                    #ifdef HASHTABLE
                                    /*enter data in the hashtable*/
                                    if(depth>=HASHTABLE_ENTRY_DEPTH)
                                    {
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                    }
                                    #endif
                                    if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = square+dir_bP[y];
                                        best_move->special = i-6;
                                    }
                                    min = (eval < min ? eval : min);
                                }

                                /*change Position back*/
                                board[square]=11;
                                board[square+dir_bP[y]]=copy_piece;

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                            }else {
                                /* If King is captured game is over*/
                                if(board[square+dir_bP[y]] == 6)
                                    return ((INT_MIN + MAXDEPTH) - depth);

                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                /* capture */
                                change_pos(board, square+dir_bP[y], square, &copy_piece, &hashkey_next_pos);

                                /* after white move -> black move -> false */
                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos); 
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_bP[y];
                                    best_move->special = 0;
                                }
                                min = (eval < min ? eval : min);

                                change_pos_back(board, square+dir_bP[y], square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                            }

                        }
                    }
                    if( (enpassant_square - square) == 1 ||
                        (enpassant_square - square) == -1  ){
                        #ifdef HASHTABLE
                        hashkey_next_pos=hashkey_pos;
                        #endif
                        /* en passant */
                        change_pos_enpassant_black(board, enpassant_square, square, &copy_piece, &hashkey_next_pos);
                        /* after white move -> black move -> false */
                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[1], hashkey_next_pos);
                        #ifdef HASHTABLE
                        /*enter data in the hashtable*/
                        if(depth>=HASHTABLE_ENTRY_DEPTH)
                        {
                            transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                            transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                            transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                        }
                        #endif
                        if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = enpassant_square + 10;
                            best_move->special = 1;
                        }
                        min = (eval < min ? eval : min);

                        change_pos_back_enpassant_black(board, enpassant_square, square, copy_piece );

                        #ifdef AB_PRUNE
                        beta = (beta < eval ? beta : eval);
                        if (beta <= alpha)
                            return min;
                        #endif

                    }
                    break;
                case 12:/* Knight */
                        for(int y = 1; y <= dir_Kn[0]; y++) {
                            /* move or capture */
                            if(board[square+dir_Kn[y]] == 0 || (board[square+dir_Kn[y]] < 10 && board[square+dir_Kn[y]] > 0)) {
                                /* If King is captured game is over*/
                                if(board[square+dir_Kn[y]] == 6)
                                    return ((INT_MIN + MAXDEPTH) - depth);

                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                change_pos(board, square+dir_Kn[y], square, &copy_piece, &hashkey_next_pos);

                                /* after white move -> black move -> false */
                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_Kn[y];
                                    best_move->special = 0;
                                }
                                min = (eval < min ? eval : min);

                                change_pos_back(board, square+dir_Kn[y], square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                            }
                        }
                        break;
                case 13:/* Bishop */
                        copy_pos = square;
                        for(int y = 1; y <= dir_B[0]; y++) {
                            /* move or capture */
                            while(board[copy_pos+dir_B[y]] == 0 || (board[copy_pos+dir_B[y]] < 10 && board[copy_pos+dir_B[y]] > 0)) {
                                if (board[copy_pos+dir_B[y]] < 10 && board[copy_pos+dir_B[y]] > 0) {
                                    /* If King is captured game is over*/
                                    if(board[copy_pos+dir_B[y]] == 6)
                                        return ((INT_MIN + MAXDEPTH) - depth);

                                    #ifdef HASHTABLE
                                    hashkey_next_pos=hashkey_pos;
                                    #endif
                                    change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece, &hashkey_next_pos);

                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                    #ifdef HASHTABLE
                                    /*enter data in the hashtable*/
                                    if(depth>=HASHTABLE_ENTRY_DEPTH)
                                    {
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                    }
                                    #endif
                                    if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = copy_pos+dir_B[y];
                                        best_move->special = 0;
                                    }
                                    min = (eval < min ? eval : min);

                                    change_pos_back(board, (copy_pos+dir_B[y]), square , copy_piece);

                                    #ifdef AB_PRUNE
                                    beta = (beta < eval ? beta : eval);
                                    if (beta <= alpha)
                                        return min;
                                    #endif

                                    /* If Piece is captured, it cant move further */
                                    break;
                                }
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                /* move */
                                change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece, &hashkey_next_pos);
                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_B[y];
                                    best_move->special = 0;
                                }
                                min = (eval < min ? eval : min);

                                change_pos_back(board, (copy_pos+dir_B[y]), square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                                copy_pos += dir_B[y];
                            }
                            copy_pos = square;
                        }
                        break;
                case 14:/* Rook */
                        copy_pos = square;
                        for(int y = 1; y <= dir_R[0]; y++) {
                            /* move or capture */
                            while(board[copy_pos+dir_R[y]] == 0 || (board[copy_pos+dir_R[y]] < 10 && board[copy_pos+dir_R[y]] > 0)) {
                                if (board[copy_pos+dir_R[y]] < 10 && board[copy_pos+dir_R[y]] > 0) {
                                    /* If King is captured game is over*/
                                    if(board[copy_pos+dir_R[y]] == 6)
                                        return ((INT_MIN + MAXDEPTH) - depth);

                                    #ifdef HASHTABLE
                                    hashkey_next_pos=hashkey_pos;
                                    #endif
                                    change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece, &hashkey_next_pos);

                                    /* If the Rook moves from it's starting square, castles to this side becomes illegal*/
                                    if(long_castle_b==1 && square == 21){
                                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                    }else if(short_castle_b==1 &&  square == 28){
                                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, false, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                    }else{
                                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                    } 
                                    #ifdef HASHTABLE
                                    /*enter data in the hashtable*/
                                    if(depth>=HASHTABLE_ENTRY_DEPTH)
                                    {
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                    }
                                    #endif

                                    if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = copy_pos+dir_R[y];
                                        best_move->special = 0;
                                    }
                                    min = (eval < min ? eval : min);

                                    change_pos_back(board, (copy_pos+dir_R[y]), square , copy_piece);

                                    #ifdef AB_PRUNE
                                    beta = (beta < eval ? beta : eval);
                                    if (beta <= alpha)
                                        return min;
                                    #endif

                                    /* If Piece is captured, it cant move further */
                                    break;
                                }
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                /* move */
                                change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece, &hashkey_next_pos);

                                /* If the Rook moves from it's starting square, castles to this side becomes illegal*/
                                if(long_castle_b==1 && square == 21){
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                                }else if(short_castle_b==1 && square == 28){
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, false, NULL, material, hashkey_next_pos);
                                }else{
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                                }
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif 

                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_R[y];
                                    best_move->special = 0;
                                }
                                min = (eval < min ? eval : min);

                                change_pos_back(board, (copy_pos+dir_R[y]), square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                                copy_pos += dir_R[y];
                            }
                            copy_pos = square;
                        }
                        break;
                case 15:/* Queen */
                        copy_pos = square;
                        for(int y = 1; y <= dir_Q_Ki[0]; y++) {
                            /* move or capture */
                            while(board[copy_pos+dir_Q_Ki[y]] == 0 || (board[copy_pos+dir_Q_Ki[y]] < 10 && board[copy_pos+dir_Q_Ki[y]] > 0)) {
                                if (board[copy_pos+dir_Q_Ki[y]] < 10 && board[copy_pos+dir_Q_Ki[y]] > 0) {
                                    /* If King is captured game is over*/
                                    if(board[copy_pos+dir_Q_Ki[y]] == 6)
                                        return ((INT_MIN + MAXDEPTH) - depth);

                                    #ifdef HASHTABLE
                                    hashkey_next_pos=hashkey_pos;
                                    #endif
                                    change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece, &hashkey_next_pos);

                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                    #ifdef HASHTABLE
                                    /*enter data in the hashtable*/
                                    if(depth>=HASHTABLE_ENTRY_DEPTH)
                                    {
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                        transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                    }
                                    #endif
                                    if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                        best_move->special = 0;
                                    }
                                    min = (eval < min ? eval : min);

                                    change_pos_back(board, (copy_pos+dir_Q_Ki[y]), square , copy_piece);

                                    #ifdef AB_PRUNE
                                    beta = (beta < eval ? beta : eval);
                                    if (beta <= alpha)
                                        return min;
                                    #endif

                                    /* If Piece is captured, it cant move further */
                                    break;
                                }
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                /* move */
                                change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece, &hashkey_next_pos);

                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL, material, hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                    best_move->special = 0;
                                }
                                min = (eval < min ? eval : min);

                                change_pos_back(board, (copy_pos+dir_Q_Ki[y]), square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                                copy_pos += dir_Q_Ki[y];
                            }
                            copy_pos = square;
                        }
                        break;
                case 16:/* KING */
                        for(int y = 1; y < dir_Q_Ki[0]; y++) {
                            if(board[square+dir_Q_Ki[y]] == 0 || ((board[square+dir_Q_Ki[y]] < 10) && board[square+dir_Q_Ki[y]] > 0)) {
                                /* If King is captured game is over*/
                                if(board[square+dir_Q_Ki[y]] == 6)
                                    return ((INT_MIN + MAXDEPTH) - depth);
                                
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                #endif
                                change_pos(board, square+dir_Q_Ki[y], square, &copy_piece, &hashkey_next_pos);

                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, false, NULL, material-material_list[copy_piece], hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_Q_Ki[y];
                                    best_move->special = 0;
                                }
                                min = (eval < min ? eval : min);

                                change_pos_back(board,square+dir_Q_Ki[y],square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                            }
                        }
                        if(long_castle_b==1 &&square==25 && board[24]==0 && board[23]==0 && board[22]==0 && board[21]==14)
                        {
                            /*testing if the King is in check or moves thew check, by giving the opponent the move*/
                            int in_check=0;
                            
                            in_check=move_gen(board, 1, INT_MIN, INT_MAX, false, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                            board[25]=0;
                            board[24]=16;
                            in_check|=move_gen(board, 1, INT_MIN, INT_MAX, false, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                            board[25]=16;
                            board[24]=0;

                            if(in_check<50000)
                            {
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                hashkey_next_pos=hashkey_next_pos^random_number[21*14];
                                hashkey_next_pos=hashkey_next_pos^random_number[23*16];
                                hashkey_next_pos=hashkey_next_pos^random_number[24*14];
                                hashkey_next_pos=hashkey_next_pos^random_number[25*16];
                                #endif
                                board[21]=0;
                                board[23]=16;
                                board[24]=14;
                                board[25]=0;

                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, false, NULL, material, hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = 0;
                                    best_move->end_pos = 0;
                                    best_move->special = 3;
                                }
                                min = (eval < min ? eval : min);

                                board[21]=14;
                                board[23]=0;
                                board[24]=0;
                                board[25]=16;

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif
                            }
                        }
                        if(short_castle_b==1 &&square==25 && board[26]==0 && board[27]==0 && board[28]==14)
                        {
                            /*testing if the King is in check or moves thew check, by giving the opponent the move*/
                            int in_check=0;
                            
                            in_check=move_gen(board, 1, INT_MIN, INT_MAX, false, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                            board[25]=0;
                            board[26]=16;
                            in_check|=move_gen(board, 1, INT_MIN, INT_MAX, false, false, false, false, false, false, NULL, 0, hashkey_next_pos);
                            board[25]=16;
                            board[26]=0;

                            if(in_check<50000)
                            {
                                #ifdef HASHTABLE
                                hashkey_next_pos=hashkey_pos;
                                hashkey_next_pos=hashkey_next_pos^random_number[25*16];
                                hashkey_next_pos=hashkey_next_pos^random_number[26*14];
                                hashkey_next_pos=hashkey_next_pos^random_number[27*16];
                                hashkey_next_pos=hashkey_next_pos^random_number[28*14];
                                #endif
                                board[25]=0;
                                board[26]=14;
                                board[27]=16;
                                board[28]=0;

                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, false, NULL, material, hashkey_next_pos);
                                #ifdef HASHTABLE
                                /*enter data in the hashtable*/
                                if(depth>=HASHTABLE_ENTRY_DEPTH)
                                {
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].depth=depth;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].eval=eval;
                                    transpositontable_black[hashkey_next_pos%HASHTABLE_SIZE].hashkey=hashkey_next_pos;
                                }
                                #endif
                                if((depth == MAXDEPTH) && (eval < min) && (best_move != NULL)){
                                    best_move->start_pos = 0;
                                    best_move->end_pos = 0;
                                    best_move->special = 5;
                                }
                                min = (eval < min ? eval : min);

                                board[25]=16;
                                board[26]=0;
                                board[27]=0;
                                board[28]=14;

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif
                            }
                        }
                        break;
                default:break;
            }
        }
        return min;
    }  
}
void change_pos(int *board, int new_pos, int old_pos, int *copy_piece, long long unsigned int *hashkey_next_pos)
{
    #ifdef HASHTABLE
    if(board[new_pos>0])
    {
        /*first we remove the captured pice if there is one from the Hashkey*/
        *hashkey_next_pos=*hashkey_next_pos^random_number[board[new_pos]*new_pos];   
    }
    /*remove the pice from the starting pos and add the pice to the new pos*/
    *hashkey_next_pos=*hashkey_next_pos^random_number[board[old_pos]*old_pos];
    *hashkey_next_pos=*hashkey_next_pos^random_number[board[old_pos]*new_pos];
    #endif
    /* save the piece to capture (on the new pos) so it can be returned */
    *copy_piece = board[new_pos];
    /* move the piece from the old position to the new position */
    board[new_pos] = board[old_pos];
    /* now the old position is empty */
    board[old_pos] = 0;
    
}
void change_pos_back(int *board, int new_pos, int old_pos, int copy_piece)
{
    /* put the moved piece back on its old position */
    board[old_pos] = board[new_pos];
    /* put the captured piece back on its position */
    board[new_pos] = copy_piece;   
}
void change_pos_enpassant_white(int *board, int new_pos, int old_pos, int *copy_piece, long long unsigned int *hashkey_next_pos)
{
    #ifdef HASHTABLE
    /*removes both pawns from the hashkey, adds the white pawn on its new square to the hashkey*/    
    *hashkey_next_pos=*hashkey_next_pos^random_number[1*new_pos];   
    *hashkey_next_pos=*hashkey_next_pos^random_number[11*old_pos];
    *hashkey_next_pos=*hashkey_next_pos^random_number[1*new_pos-10];
    #endif
    /* save the piece to capture (on the new pos) so it can be returned */
    *copy_piece = board[new_pos - 10];
    /* move the piece from the old position to the new position */
    board[new_pos - 10] = board[old_pos];
    /* now the old position is empty */
    board[old_pos] = 0;
    /* remove the captured pawn */
    board[new_pos] = 0;
}
void change_pos_back_enpassant_white(int *board, int new_pos, int old_pos,int copy_piece)
{
    /* move piece back to old spot */
    board[old_pos] = board[new_pos - 10];
    /* set the square to zero */
    board[new_pos - 10] = copy_piece;
    /* put the captured piece back on the board */
    board[new_pos] = 11;
}
void change_pos_enpassant_black(int *board, int new_pos, int old_pos, int *copy_piece, long long unsigned int *hashkey_next_pos)
{
    #ifdef HASHTABLE
    /*removes both pawns from the hashkey, adds the black pawn on its new square to the hashkey*/    
    *hashkey_next_pos=*hashkey_next_pos^random_number[11*new_pos];   
    *hashkey_next_pos=*hashkey_next_pos^random_number[1*old_pos];
    *hashkey_next_pos=*hashkey_next_pos^random_number[111*new_pos+10];
    #endif
    /* save the piece to capture (on the new pos) so it can be returned */
    *copy_piece = board[new_pos + 10];
    /* move the piece from the old position to the new position */
    board[new_pos + 10] = board[old_pos];
    /* now the old position is empty */
    board[old_pos] = 0;
    /* remove the captured pawn */
    board[new_pos] = 0;
}
void change_pos_back_enpassant_black(int *board, int new_pos, int old_pos,int copy_piece)
{
    /* move piece back to old spot */
    board[old_pos] = board[new_pos + 10];
    /* set the square to zero */
    board[new_pos + 10] = copy_piece;
    /* put the captured piece back on the board */
    board[new_pos] = 1;
}
