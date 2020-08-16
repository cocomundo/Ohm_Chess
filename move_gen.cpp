//#include <iomanip>
#include <iostream>
#include <limits.h> // INT_MIN, INT_MAX

#include "move_gen.h"
#include "evaluation.h"


//directions, first number of array represents the amount of directions
int dir_B[5]={4,-9,9,-11,11};
int dir_R[5]={4,-1,1,-10,10};
int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
int dir_wP[5]={4,-10,-20,-9,-11};
int dir_bP[5]={4,10,20,9,11};

int move_gen( int board[], int depth, int alpha, int beta, bool black_or_white ) {
    if ( depth == 0 ) {
        return evaluation(board);
    }
    int eval = 0;
    int copy_piece = 0;
    int copy_pos = 0;
    int enpassant_square_black = -1;
    int enpassant_square_white = -1;

    if( black_or_white) { //white
        int max = INT_MIN;
                         // min/max compare value muss an die piece_functions nicht übergeben werden,
                         // da der return value direkt nach aufruf nochmal gecheckt wird (vgl. max = (eval > max ? eval : max))

        for (int square = 20; square < 100; square++){
            switch (board[square]) {
                case 1: // Pawn
                        if(board[square+dir_wP[1]] == 0) {
                            if(square+dir_wP[1] < 30) {
                                // promotion
                                change_pos(board, square+dir_wP[1], square, &copy_piece);
                                // decide between Queen, Rook, Bishop and Knight what piece is the best
                                for(int i = 2; i < 6; i++)
                                {
                                    board[square] = i;
                                    eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move -> false
                                    max = (eval > max ? eval : max);
                                }
                                change_pos_back(board, square+dir_wP[1], square, copy_piece);
                            }else if((board[square+dir_wP[2]]==0) && square > 80) {
                                // move two squares, if still on 2nd rang
                                change_pos(board, square+dir_wP[2], square, &copy_piece);
                                enpassant_square_white = square+dir_wP[2];

                                eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move -> false
                                max = (eval > max ? eval : max);

                                change_pos_back(board, square+dir_wP[2], square, copy_piece);
                            }else {
                                // move one square
                                change_pos(board, square+dir_wP[1], square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move -> false
                                max = (eval > max ? eval : max);

                                change_pos_back(board, square+dir_wP[1], square, copy_piece);
                            }
                        }
                        for(int y = 2; y < 4; y++) {
                            if(board[square+dir_wP[y]] > 10) {
                                if(square+dir_wP[y] < 30) {
                                    //capture into promotion
                                    change_pos(board, square+dir_wP[y], square, &copy_piece);
                                    for(int i = 2; i < 6; i++)
                                    {
                                        board[square] = i;
                                        eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move -> false
                                        max = (eval > max ? eval : max);
                                    }
                                    change_pos_back(board, square+dir_wP[y], square, copy_piece);
                                }else {
                                    //capture
                                    change_pos(board, square+dir_wP[y], square, &copy_piece);
                                    eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move -> false
                                    max = (eval > max ? eval : max);

                                    change_pos_back(board, square+dir_wP[y], square, copy_piece);
                                }

                            }
                        }
                        if( ( (enpassant_square_black - square)== 1 ||
                              (enpassant_square_black - square) == -1 ) &&
                              (board[enpassant_square_black - 10] == 0) ){
                            // en passant
                            change_pos_enpassant_white(board, enpassant_square_black, square, &copy_piece );
                            eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move -> false
                            max = (eval > max ? eval : max);
                            change_pos_back_enpassant_white(board, enpassant_square_black, square, copy_piece );
                        }
                        break;
                case 2: // Knight
                        for(int y = 1; y <= dir_Kn[0]; y++) {
                            //move or capture
                            if(board[square+dir_Kn[y]] == 0 || board[square+dir_Kn[y]] > 10) {
                                change_pos(board, square+dir_Kn[y], square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move -> false
                                max = (eval > max ? eval : max);
/*
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    square = 100;
*/

                                change_pos_back(board, square+dir_Kn[y], square, copy_piece);
                            }
                        }
                        std::cout<< "return max: " << max <<" eval after: " << eval << std::endl;
                        break;
                case 3: // Bishop
                         copy_pos = square; 
                        for(int y = 1; y <= dir_B[0]; y++) { // check every direction the piece can go
                            while(board[copy_pos+dir_B[y]] == 0 || board[copy_pos+dir_B[y]] > 10) { // move or capture
                                if (board[copy_pos+dir_B[y]] > 10) { // capture
                                    change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece);

                                    eval = move_gen(board, depth - 1, alpha, beta, false); //after white move -> black move
                                    max = (eval > max ? eval : max);
/*
                                    alpha = (alpha > eval ? alpha : eval);
                                    if (beta <= alpha)
                                        square = 100;
*/

                                    change_pos_back(board, (copy_pos+dir_B[y]), square, copy_piece);
                                    // Stop, because a piece is captured
                                    break; 
                                } 
                                change_pos(board, copy_pos+dir_B[y], square, &copy_piece);

                                eval = move_gen(board, depth -1, alpha, beta, false);
                                max = (eval > max ? eval : max);
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    square = 100;

                                change_pos_back(board, copy_pos+dir_B[y], square, copy_piece);
                                copy_pos += dir_B[y];
                            }
                            //after each direction is done, the copy pos needs to be resettet
                            copy_pos = square;
                        }
                        std::cout<< "return max: " << max <<" eval after: " << eval << std::endl;
                        break;
                case 4: // Rook
                        copy_pos = square; 
                        for(int y = 1; y <= dir_R[0]; y++) { // check every direction the piece can go
                            while(board[copy_pos+dir_R[y]] == 0 || board[copy_pos+dir_R[y]] > 10) { // move or capture
                                if (board[copy_pos+dir_R[y]] > 10) { // capture
                                    change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece);

                                    eval = move_gen(board, depth - 1, alpha, beta, false); //after white move -> black move
                                    max = (eval > max ? eval : max);
/*
                                    alpha = (alpha > eval ? alpha : eval);
                                    if (beta <= alpha)
                                        square = 100;
*/

                                    change_pos_back(board, (copy_pos+dir_R[y]),square , copy_piece);
                                    break; // Stop, because a piece is captured
                                } 
                                change_pos(board, copy_pos+dir_R[y], square, &copy_piece);

                                eval = move_gen(board, depth -1, alpha, beta, false);
                                max = (eval > max ? eval : max);
/*
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    square = 100;
*/

                                change_pos_back(board, copy_pos+dir_R[y], square, copy_piece);
                                copy_pos += dir_R[y];
                            }
                            //after each direction is done, the copy pos needs to be resettet
                            copy_pos=square;
                        }
                        std::cout<< "return max: " << max <<" eval after: " << eval << std::endl;
                        break;
                case 5: // Queen
                        copy_pos = square; 
                        for(int y = 1; y <= dir_Q_Ki[0]; y++) { // check every direction the piece can go
                            while(board[copy_pos+dir_Q_Ki[y]] == 0 || board[copy_pos+dir_Q_Ki[y]] > 10) { // move or capture
                                if (board[copy_pos+dir_Q_Ki[y]] > 10) { // capture
                                    change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece);

                                    eval = move_gen(board, depth - 1, alpha, beta, false); //after white move -> black move
                                    max = (eval > max ? eval : max);
/*
                                    alpha = (alpha > eval ? alpha : eval);
                                    if (beta <= alpha)
                                        square = 100;
*/

                                    change_pos_back(board, (copy_pos+dir_Q_Ki[y]),square , copy_piece);
                                    break; // Stop, because a piece is captured
                                } 
                                change_pos(board, copy_pos+dir_Q_Ki[y], square, &copy_piece);

                                eval = move_gen(board, depth -1, alpha, beta, false);
                                max = (eval > max ? eval : max);
                                alpha = (alpha > eval ? alpha : eval);
                                if (beta <= alpha)
                                    square = 100;

                                change_pos_back(board, copy_pos+dir_Q_Ki[y], square, copy_piece);
                                copy_pos += dir_Q_Ki[y];
                            }
                            //after each direction is done, the copy pos needs to be resettet
                            copy_pos = square;
                        }
                        std::cout<< "return max: " << max <<" eval after: " << eval << std::endl;
                        break;
                //case 6:white_Ki(long_castle_w, short_castle_w, i, dir_Q_Ki, depth);//Knight and King have same move-function but different directions
                       // max = (eval > max ? eval : max);
                 //       break;
                default:break;
            }
        }
        std::cout<< "return max: (FIN)" << max << std::endl;
        return max;

    } else {
        int min = INT_MAX;        

        for (int square = 20; square < 100; square++) {
            switch (board[square]) {
                case 11:// Pawn
                        if(board[square+dir_bP[1]] == 0) {
                            if(square+dir_bP[1] > 90) {
                                // promotion
                                change_pos(board, square+dir_bP[1], square, &copy_piece);
                                // decide between Queen, Rook, Bishop and Knight what piece is the best
                                for(int i = 12; i < 16; i++)
                                {
                                    board[square] = i;
                                    eval = move_gen(board, depth-1, alpha, beta, true); //after black move -> white move -> true
                                    min = (eval < min ? eval : min);
                                }
                                change_pos_back(board, square+dir_wP[1], square, copy_piece);
                            }else if((board[square+dir_wP[2]] == 0) && square < 40) {
                                // move two squares, if still on 2nd rang
                                change_pos(board, square+dir_wP[2], square, &copy_piece);
                                enpassant_square_black = square+dir_wP[2];
                                eval = move_gen(board, depth-1, alpha, beta, true); //after black move -> white move -> true
                                min = (eval < min ? eval : min);
                                change_pos_back(board, square+dir_wP[2], square, copy_piece);
                            }else {
                                // move one square
                                change_pos(board, square+dir_wP[1], square, &copy_piece);
                                eval = move_gen(board, depth-1, alpha, beta, true); //after black move -> white move -> true
                                min = (eval < min ? eval : min);
                                change_pos_back(board, square+dir_wP[1], square, copy_piece);
                            }
                        }
                        for(int y = 2; y < 4; y++) {
                            if(board[square+dir_wP[y]] < 10 && board[square+dir_wP[y]] > 0) {
                                if(square+dir_wP[y] > 90) {
                                    //capture into promotion
                                    change_pos(board, square+dir_wP[y], square, &copy_piece);
                                    for(int i = 12; i < 16; i++)
                                    {
                                        board[square] = i;
                                        eval = move_gen(board, depth-1, alpha, beta, true); //after black move -> white move -> true
                                        min = (eval < min ? eval : min);
                                    }
                                    change_pos_back(board, square+dir_wP[y], square, copy_piece);
                                }else {
                                    //capture
                                    change_pos(board, square+dir_wP[y], square, &copy_piece);
                                    eval = move_gen(board, depth-1, alpha, beta, true); //after white move -> black move -> false
                                    min = (eval < min ? eval : min);
                                    change_pos_back(board, square+dir_wP[y], square, copy_piece);
                                }

                            }
                        }
                        if( ( (enpassant_square_white - square)== 1 ||
                              (enpassant_square_white - square) == -1 ) &&
                              (board[enpassant_square_white - 10] == 0) ){
                            // en passant
                            change_pos_enpassant_white(board, enpassant_square_black, square, &copy_piece );
                            eval = move_gen(board, depth-1, alpha, beta, true); //after white move -> black move -> false
                            min = (eval < min ? eval : min);
                            change_pos_back_enpassant_white(board, enpassant_square_black, square, copy_piece );
                        }
                        break;
                case 12:// Knight
                        for(int y = 1; y <= dir_Kn[0]; y++) {
                            //move or capture
                            if(board[square+dir_Kn[y]] == 0 || (board[square+dir_Kn[y]] < 10 && board[square+dir_Kn[y]] > 0)) {
                                change_pos(board, square+dir_Kn[y], square, &copy_piece);

                                eval = move_gen(board, depth-1, alpha, beta, true); //after white move -> black move -> false
                                min = (eval < min ? eval : min);
/*
                                beta = (beta < eval ? beta : eval);
                                if (beta <= alpha)
                                    square = 100;
*/
                                change_pos_back(board, square+dir_Kn[y], square, copy_piece);
                            }
                        }
                        std::cout<< "return min: " << min <<" eval after: " << eval << std::endl;
                        break;
                case 13:// Bishop
                        copy_pos = square;
                        for(int y = 1; y <= dir_B[0]; y++) {
                            while(board[copy_pos+dir_B[y]] == 0 || (board[copy_pos+dir_B[y]] < 10 && board[copy_pos+dir_B[y]] > 0)) { // move or capture
                                if (board[copy_pos+dir_B[y]] < 10 && board[copy_pos+dir_B[y]] > 0) {
                                    change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece);

                                    eval = move_gen(board, depth -1, alpha, beta, true);
                                    min = (eval < min ? eval : min);
/*
                                    beta = (beta < eval ? beta : eval);
                                    if( beta <= alpha)
                                        square = 100;
*/

                                    change_pos_back(board, (copy_pos+dir_B[y]), square , copy_piece);
                                    break;//If Piece is captured, it cant move further
                                }
                                change_pos(board, (copy_pos+dir_B[y]), square, &copy_piece);

                                eval = move_gen(board, depth -1, alpha, beta, true);
                                min = (eval < min ? eval : min);
                                beta = (beta < eval ? beta : eval);
                                if( beta <= alpha)
                                    square = 100;

                                change_pos_back(board, (copy_pos+dir_B[y]), square, copy_piece);

                                copy_pos += dir_B[y];
                            }
                            copy_pos = square;
                        }
                        std::cout<< "return min: " << min <<" eval after: " << eval << std::endl;
                        break;
                case 14:// Rook
                        copy_pos = square;
                        for(int y = 1; y <= dir_R[0]; y++) {
                            while(board[copy_pos+dir_R[y]] == 0 || (board[copy_pos+dir_R[y]] < 10 && board[copy_pos+dir_R[y]] > 0)) { // move or capture
                                if (board[copy_pos+dir_R[y]] < 10 && board[copy_pos+dir_R[y]] > 0) {
                                    change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece);

                                    eval = move_gen(board, depth -1, alpha, beta, true);
                                    min = (eval < min ? eval : min);
                                    beta = (beta < eval ? beta : eval);
                                    if( beta <= alpha)
                                        square = 100;

                                    change_pos_back(board, (copy_pos+dir_R[y]), square , copy_piece);
                                    break;//If Piece is captured, it cant move further
                                }
                                change_pos(board, (copy_pos+dir_R[y]), square, &copy_piece);

                                eval = move_gen(board, depth -1, alpha, beta, true);
                                min = (eval < min ? eval : min);
/*
                                beta = (beta < eval ? beta : eval);
                                if( beta <= alpha)
                                    square = 100;
*/

                                change_pos_back(board, (copy_pos+dir_R[y]), square, copy_piece);
                                copy_pos += dir_R[y];
                            }
                            copy_pos = square;
                        }
                        std::cout<< "return min: " << min <<" eval after: " << eval << std::endl;
                        break;
                case 15:// Queen
                        copy_pos = square;
                        for(int y = 1; y <= dir_Q_Ki[0]; y++) {
                            while(board[copy_pos+dir_Q_Ki[y]] == 0 || (board[copy_pos+dir_Q_Ki[y]] < 10 && board[copy_pos+dir_Q_Ki[y]] > 0)) { // move or capture
                                if (board[copy_pos+dir_Q_Ki[y]] < 10 && board[copy_pos+dir_Q_Ki[y]] > 0) {
                                    change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece);

                                    eval = move_gen(board, depth -1, alpha, beta, true);
                                    min = (eval < min ? eval : min);
/*
                                    beta = (beta < eval ? beta : eval);
                                    if( beta <= alpha)
                                        square = 100;
*/

                                    change_pos_back(board, (copy_pos+dir_Q_Ki[y]), square , copy_piece);
                                    break;//If Piece is captured, it cant move further
                                }
                                change_pos(board, (copy_pos+dir_Q_Ki[y]), square, &copy_piece);

                                eval = move_gen(board, depth -1, alpha, beta, true);
                                min = (eval < min ? eval : min);
/*
                                beta = (beta < eval ? beta : eval);
                                if( beta <= alpha)
                                    square = 100;
*/

                                change_pos_back(board, (copy_pos+dir_Q_Ki[y]), square, copy_piece);
                                copy_pos += dir_Q_Ki[y];
                            }
                            copy_pos = square;
                        }
                        std::cout<< "return min: " << min <<" eval after: " << eval << std::endl;
                        break;
                //case 16:black_Ki( long_castle_b, short_castle_b, i, dir_Q_Ki, depth);
                        //min = (eval > min ? eval : min);
                  //      break;
                default:break;
            }
        }
        std::cout<< "return min: (FIN)" << min << std::endl;
        return min;
    }  
}
void change_pos(int board[], int new_pos, int old_pos, int *copy_piece)
{
    // save the piece to capture (on the new pos) so it can be returned
    *copy_piece = board[new_pos];
    // move the piece from the old position to the new position
    board[new_pos] = board[old_pos];
    // now the old position is empty
    board[old_pos] = 0;
    printf("move\n");
}
void change_pos_back(int board[], int new_pos, int old_pos, int copy_piece)
{
    board[old_pos] = board[new_pos];
    board[new_pos] = copy_piece;   
    printf("back\n");
}
void change_pos_enpassant_white(int board[], int new_pos, int old_pos, int *copy_piece)
{
    // save the piece to capture (on the new pos) so it can be returned
    *copy_piece = board[new_pos - 10];
    // move the piece from the old position to the new position
    board[new_pos - 10] = board[old_pos];
    // now the old position is empty
    board[old_pos] = 0;
    // remove the captured pawn
    board[new_pos] = 0;

    printf("move enpassant\n");
}
void change_pos_back_enpassant_white(int board[], int new_pos, int old_pos,int copy_piece)
{
    // move piece back to old spot
    board[old_pos] = board[new_pos - 10];
    // set the square to zero
    board[new_pos - 10] = copy_piece;
    // put the captured piece back on the board
    board[new_pos] = 11;
    printf("back enpassant\n");
}
void change_pos_enpassant_black(int board[], int new_pos, int old_pos, int *copy_piece)
{
    // save the piece to capture (on the new pos) so it can be returned
    *copy_piece = board[new_pos + 10];
    // move the piece from the old position to the new position
    board[new_pos + 10] = board[old_pos];
    // now the old position is empty
    board[old_pos] = 0;
    // remove the captured pawn
    board[new_pos] = 0;

    printf("move enpassant\n");
}
void change_pos_back_enpassant_black(int board[], int new_pos, int old_pos,int copy_piece)
{
    // move piece back to old spot
    board[old_pos] = board[new_pos + 10];
    // set the square to zero
    board[new_pos + 10] = copy_piece;
    // put the captured piece back on the board
    board[new_pos] = 1;
    printf("back enpassant\n");
}
/*
int white_Ki(int board[], bool long_castle_w, bool short_castle_w, int pos,int dir[], int depth) {
    int copy_piece;
    for(int y = 1; y <= dir[0]; y++) { //first position of "dir" gives amount of directions
        if(board[pos+dir[y]] == 0 || board[pos+dir[y]] > 10) {// move or capture
            change_pos(board, pos+dir[y], pos, &copy_piece);

            eval = move_gen(board, depth-1, false); //after white move -> black move
            if(eval > max) {
                max = eval;
            }

            change_pos_back(board, pos+dir[y], pos, copy_piece);
        }
    }

    if(long_castle_w==1&&pos==95&&board[94]==0&&board[93]==0&&board[92]==0&&board[91]==4)
    {

        board[91]=0;
        board[93]=6;
        board[94]=4;
        board[95]=0;
        if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
        board[91]=4;
        board[93]=0;
        board[94]=0;
        board[95]=6;
    }
    if(short_castle_w==1&&pos==95&&board[96]==0&&board[97]==0&&board[98]==4)
    {

        board[95]=0;
        board[96]=4;
        board[97]=6;
        board[98]=0;
        if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
        board[95]=6;
        board[96]=0;
        board[97]=0;
        board[98]=4;
    }

}*/
