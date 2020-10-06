#include <stdio.h>
#include <limits.h> /* INT_MIN, INT_MAX */

#include "move_gen.h"
#include "evaluation.h"


/* directions, first number of array represents the amount of directions */
int dir_B[5]={4,-9,9,-11,11};
int dir_R[5]={4,-1,1,-10,10};
int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
int dir_wP[5]={4,-10,-20,-9,-11};
int dir_bP[5]={4,10,20,9,11};

double count = 0;

int move_gen( int *board, int depth, int alpha, int beta, bool black_or_white,
    int enpassant_square, bool long_castle_w, bool long_castle_b,
    bool short_castle_w, bool short_castle_b, MOVE *best_move )
{
    if ( depth == 0 ) {
        #ifdef BENCHMARK
        count += 1;
        #endif
        return evaluation(board);
    }
    int eval = 0; /* stores the evaluation value of the calculated move */
    int copy_piece = 0; /* dummy var to store the captured piece */
    int copy_pos = 0; /* dummy var to store the previous square */

    /* white*/
    if(black_or_white) { 
        int max = INT_MIN;

        for (int square = 20; square < 100; square++){
            switch (board[square]) {
                case 1: /* Pawn */
                    if(board[square+dir_wP[1]] == 0) {
                        if(square+dir_wP[1] < 30) {
                            /* promotion */
                            change_pos(board, square+dir_wP[1], square, &copy_piece);

                            /* convert the pawn into another piece */
                            for(int i = 2; i < 6; i++)
                            {
                                board[square] = i;
                                /* after white move -> black move -> false */
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                max = (eval > max ? eval : max);
                                if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_wP[1];
                                    best_move->special = i+4;
                                }
                            }
                            change_pos_back(board, square+dir_wP[1], square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif

                        }
                        if(square > 80 && (board[square+dir_wP[2]] == 0)) {

                            /* move two squares, if still on 2nd rang */
                            change_pos(board, square+dir_wP[2], square, &copy_piece);

                            /* after white move -> black move -> false */
                            eval = move_gen(board, depth-1, alpha, beta, false, square+dir_wP[2], long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL );
                            max = (eval > max ? eval : max);
                            if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = square+dir_wP[2];
                                best_move->special = 0;
                            }

                            change_pos_back(board, square+dir_wP[2], square, copy_piece);

                            #ifdef AB_PRUNE
                            alpha = (alpha > eval ? alpha : eval);
                            if (beta <= alpha)
                                return max;
                            #endif
                        }
                        /* move one square */
                        change_pos(board, square+dir_wP[1], square, &copy_piece);

                        /* after white move -> black move -> false */
                        eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                        max = (eval > max ? eval : max);
                        if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = square+dir_wP[1];
                            best_move->special = 0;
                        }

                        change_pos_back(board, square+dir_wP[1], square, copy_piece);

                        #ifdef AB_PRUNE
                        alpha = (alpha > eval ? alpha : eval);
                        if (beta <= alpha)
                            return max;
                        #endif
                    }
                    for(int y = 3; y < 5; y++) {
                        if(board[square+dir_wP[y]] > 10) {
                            if(square+dir_wP[y] < 30) {
                                /* If King is captured game is over*/
                                if(board[square+dir_wP[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);

                                /*capture into promotion */
                                change_pos(board, square+dir_wP[y], square, &copy_piece);

                                /* convert the pawn into another piece */
                                for(int i = 2; i < 6; i++)
                                {
                                    board[square] = i;
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                    max = (eval > max ? eval : max);
                                    if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = square+dir_wP[y];
                                        best_move->special = 4+i;
                                    }
                                }

                                change_pos_back(board, square+dir_wP[y], square, copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif
                            }else {
                                /* If King is captured game is over*/
                                if(board[square+dir_wP[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);

                                /* capture */
                                change_pos(board, square+dir_wP[y], square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                max = (eval > max ? eval : max);
                                if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_wP[y];
                                    best_move->special = 0;
                                }

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
                        /* en passant */
                        change_pos_enpassant_white(board, enpassant_square, square, &copy_piece );
                        eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                        max = (eval > max ? eval : max);
                        if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = enpassant_square - 10;
                            best_move->special = 1;
                        }

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

                            change_pos(board, square+dir_Kn[y], square, &copy_piece);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                            max = (eval > max ? eval : max);
                            if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = square+dir_Kn[y];
                                best_move->special = 0;
                            }

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
                    /* check every direction the piece can move */
                    for(int y = 1; y <= dir_B[0]; y++) { 
                        while(board[copy_pos+dir_B[y]] == 0 || board[copy_pos+dir_B[y]] > 10) {
                            /* capture */
                            if (board[copy_pos+dir_B[y]] > 10) {
                                /* If King is captured game is over*/
                                if(board[copy_pos+dir_B[y]] == 16)
                                    return ((INT_MAX - MAXDEPTH)+ depth);

                                change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                max = (eval > max ? eval : max);
                                if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_B[y];
                                    best_move->special = 0;
                                }

                                change_pos_back(board, (copy_pos+dir_B[y]), square, copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif

                                /* Stop, because a piece is captured */
                                break; 
                            } 
                            /* move */
                            change_pos(board, copy_pos+dir_B[y], square, &copy_piece);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                            max = (eval > max ? eval : max);
                            if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_B[y];
                                best_move->special = 0;
                            }

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

                                change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece);

                                /* COMMENT REQUIRED*/
                                if(square == 91){
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, short_castle_w, short_castle_b, NULL);
                                }else if(square == 98){
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, false, short_castle_b, NULL);
                                }else{
                                    eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                } 

                                max = (eval > max ? eval : max);
                                if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_R[y];
                                    best_move->special = 0;
                                }

                                change_pos_back(board, (copy_pos+dir_R[y]),square , copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif

                                /* Stop, because a piece is captured */
                                break; 
                            }
                            /* move */
                            change_pos(board, copy_pos+dir_R[y], square, &copy_piece);

                            if(square == 91){
                                eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, short_castle_w, short_castle_b, NULL);
                            }else if(square == 98){
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, false, short_castle_b, NULL);
                            }else{
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                            } 

                            max = (eval > max ? eval : max);
                            if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_R[y];
                                best_move->special = 0;
                            }

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

                                change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece);
                                /* after white move -> black move */
                                eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                max = (eval > max ? eval : max);
                                if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                    best_move->special = 0;
                                }

                                change_pos_back(board, (copy_pos+dir_Q_Ki[y]),square , copy_piece);

                                #ifdef AB_PRUNE
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    return max;
                                #endif

                                /* Stop, because a piece is captured */
                                break; 
                            } 
                            /* move */
                            change_pos(board, copy_pos+dir_Q_Ki[y], square, &copy_piece);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                            max = (eval > max ? eval : max);
                            if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                best_move->special = 0;
                            }

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
                            change_pos(board,square+dir_Q_Ki[y],square, &copy_piece);

                            eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, false, short_castle_b, NULL);
                            max = (eval > max ? eval : max);
                            if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                best_move->special = 0;
                            }

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
                        board[91]=0;
                        board[93]=6;
                        board[94]=4;
                        board[95]=0;

                        eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, false, short_castle_b, NULL);
                        max = (eval > max ? eval : max);
                        if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                            best_move->start_pos = 0;
                            best_move->end_pos = 0;
                            best_move->special = 4;
                        }

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
                    if(short_castle_w==1 &&square==95 && board[96]==0 && board[97]==0 && board[98]==4)
                    {

                        board[95]=0;
                        board[96]=4;
                        board[97]=6;
                        board[98]=0;

                        eval = move_gen(board, depth-1, alpha, beta, false, false, false, long_castle_b, false, short_castle_b, NULL);
                        max = (eval > max ? eval : max);
                        if((depth == MAXDEPTH) && (eval == max) && (best_move != NULL)){
                            best_move->start_pos = 0;
                            best_move->end_pos = 0;
                            best_move->special = 2;
                        }

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
                            change_pos(board, square+dir_bP[1], square, &copy_piece);
                            for(int i = 12; i < 16; i++)
                            {
                                board[square] = i;
                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                min = (eval < min ? eval : min);
                                if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_bP[1];
                                    best_move->special = i-6;
                                }
                            }

                            change_pos_back(board, square+dir_bP[1], square, copy_piece);

                            #ifdef AB_PRUNE
                            beta = (beta < eval ? beta : eval);
                            if (beta <= alpha)
                                return min;
                            #endif

                        }
                        if((square < 40) && (board[square+dir_bP[2]] == 0)) {
                            /* move two squares, if still on 2nd rang */
                            change_pos(board, square+dir_bP[2], square, &copy_piece);

                            /* after black move -> white move -> true */
                            eval = move_gen(board, depth-1, alpha, beta, true, square+dir_bP[2], long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL); 
                            min = (eval < min ? eval : min);
                            if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                best_move->start_pos = square;
                                best_move->end_pos = square+dir_bP[2];
                                best_move->special = 0;
                            }

                            change_pos_back(board, square+dir_bP[2], square, copy_piece);

                            #ifdef AB_PRUNE
                            beta = (beta < eval ? beta : eval);
                            if (beta <= alpha)
                                return min;
                            #endif
                        }
                        /* move one square */
                        change_pos(board, square+dir_bP[1], square, &copy_piece);

                        /* after black move -> white move -> true */
                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                        min = (eval < min ? eval : min);
                        if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = square+dir_bP[1];
                            best_move->special = 0;
                        }

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
                                /* capture into promotion */
                                change_pos(board, square+dir_bP[y], square, &copy_piece);

                                /* convert the pawn into another piece */
                                for(int i = 12; i < 16; i++)
                                {
                                    board[square] = i;
                                    /* after black move -> white move -> true */
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL); 
                                    min = (eval < min ? eval : min);
                                    if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = square+dir_bP[y];
                                        best_move->special = i-6;
                                    }
                                }

                                change_pos_back(board, square+dir_bP[y], square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                            }else {
                                /* If King is captured game is over*/
                                if(board[square+dir_bP[y]] == 6)
                                    return ((INT_MIN + MAXDEPTH) - depth);
                                /* capture */
                                change_pos(board, square+dir_bP[y], square, &copy_piece);

                                /* after white move -> black move -> false */
                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL); 
                                min = (eval < min ? eval : min);
                                if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_bP[y];
                                    best_move->special = 0;
                                }

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
                        /* en passant */
                        change_pos_enpassant_black(board, enpassant_square, square, &copy_piece );
                        /* after white move -> black move -> false */
                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                        min = (eval < min ? eval : min);
                        if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                            best_move->start_pos = square;
                            best_move->end_pos = enpassant_square + 10;
                            best_move->special = 1;
                        }

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
                                change_pos(board, square+dir_Kn[y], square, &copy_piece);

                                /* after white move -> black move -> false */
                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                min = (eval < min ? eval : min);
                                if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_Kn[y];
                                    best_move->special = 0;
                                }

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
                                    change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece);

                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                    min = (eval < min ? eval : min);
                                    if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = copy_pos+dir_B[y];
                                        best_move->special = 0;
                                    }

                                    change_pos_back(board, (copy_pos+dir_B[y]), square , copy_piece);

                                    #ifdef AB_PRUNE
                                    beta = (beta < eval ? beta : eval);
                                    if (beta <= alpha)
                                        return min;
                                    #endif

                                    /* If Piece is captured, it cant move further */
                                    break;
                                }
                                /* move */
                                change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                min = (eval < min ? eval : min);
                                if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_B[y];
                                    best_move->special = 0;
                                }

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

                                    change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece);

                                    if(square == 21){
                                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, short_castle_b, NULL);
                                    }else if(square == 28){
                                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, false, NULL);
                                    }else{
                                        eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                    } 

                                    min = (eval < min ? eval : min);
                                    if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = copy_pos+dir_R[y];
                                        best_move->special = 0;
                                    }

                                    change_pos_back(board, (copy_pos+dir_R[y]), square , copy_piece);

                                    #ifdef AB_PRUNE
                                    beta = (beta < eval ? beta : eval);
                                    if (beta <= alpha)
                                        return min;
                                    #endif

                                    /* If Piece is captured, it cant move further */
                                    break;
                                }
                                /* move */
                                change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece);

                                if(square == 21){
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, short_castle_b, NULL);
                                }else if(square == 28){
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, false, NULL);
                                }else{
                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                } 

                                min = (eval < min ? eval : min);
                                if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_R[y];
                                    best_move->special = 0;
                                }

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

                                    change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece);

                                    eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                    min = (eval < min ? eval : min);
                                    if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                        best_move->start_pos = square;
                                        best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                        best_move->special = 0;
                                    }

                                    change_pos_back(board, (copy_pos+dir_Q_Ki[y]), square , copy_piece);

                                    #ifdef AB_PRUNE
                                    beta = (beta < eval ? beta : eval);
                                    if (beta <= alpha)
                                        return min;
                                    #endif

                                    /* If Piece is captured, it cant move further */
                                    break;
                                }
                                /* move */
                                change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, long_castle_b, short_castle_w, short_castle_b, NULL);
                                min = (eval < min ? eval : min);
                                if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = copy_pos+dir_Q_Ki[y];
                                    best_move->special = 0;
                                }

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

                                change_pos(board, square+dir_Q_Ki[y], square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, false, NULL);
                                min = (eval < min ? eval : min);
                                if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                    best_move->start_pos = square;
                                    best_move->end_pos = square+dir_Q_Ki[y];
                                    best_move->special = 0;
                                }

                                change_pos_back(board,square+dir_Q_Ki[y],square, copy_piece);

                                #ifdef AB_PRUNE
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    return min;
                                #endif

                            }
                        }
                        if(long_castle_w==1 &&square==25 && board[24]==0 && board[23]==0 && board[22]==0 && board[21]==14)
                        {
                            board[21]=0;
                            board[23]=16;
                            board[24]=14;
                            board[25]=0;

                            eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, false, NULL);
                            min = (eval < min ? eval : min);
                            if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                best_move->start_pos = 0;
                                best_move->end_pos = 0;
                                best_move->special = 5;
                            }

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
                        if(short_castle_w==1 &&square==25 && board[26]==0 && board[27]==0 && board[28]==14)
                        {

                            board[25]=0;
                            board[26]=14;
                            board[27]=16;
                            board[28]=0;

                            eval = move_gen(board, depth-1, alpha, beta, true, false, long_castle_w, false, short_castle_w, false, NULL);
                            min = (eval < min ? eval : min);
                            if((depth == MAXDEPTH) && (eval == min) && (best_move != NULL)){
                                best_move->start_pos = 0;
                                best_move->end_pos = 0;
                                best_move->special = 3;
                            }

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
                        break;
                default:break;
            }
        }
        return min;
    }  
}
void change_pos(int *board, int new_pos, int old_pos, int *copy_piece)
{
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
void change_pos_enpassant_white(int *board, int new_pos, int old_pos, int *copy_piece)
{
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
void change_pos_enpassant_black(int *board, int new_pos, int old_pos, int *copy_piece)
{
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
