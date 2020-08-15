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

int move_gen( int board[], int depth, int alpha, int beta, bool black_white ) {
    if ( depth == 0 ) {
        return evaluation(board);
    }
    int eval = 0;

    if( black_white) { //white
        int max = INT_MIN; //static ?
                         // min/max compare value muss an die piece_functions nicht übergeben werden,
                         // da der return value direkt nach aufruf nochmal gecheckt wird (vgl. max = (eval > max ? eval : max))

        for (int i = 20; i < 100; i++){
            switch (board[i]) {
                /*case 1:eval = white_P( board, i, dir_wP, depth -1);
                        max = (eval > max ? eval : max);
                        break;*/
                case 2:eval = white_Kn( board, i, dir_Kn, depth, alpha, beta);
                        max = (eval > max ? eval : max);
                        alpha = (alpha > eval ? alpha : eval);
                        if (beta <= alpha)
                            i = 100;
                        std::cout<< "return max: " << max <<" eval after: " << eval << std::endl;
                        break;
                case 3:eval = white_R_Q_B( board, i, dir_B, depth, alpha, beta );
                        max = (eval > max ? eval : max);
                        alpha = (alpha > eval ? alpha : eval);
                        if (beta <= alpha)
                            i = 100;
                        std::cout<< "return max: " << max <<" eval after: " << eval << std::endl;
                        break;
                case 4:eval = white_R_Q_B( board, i, dir_R, depth, alpha, beta ); 
                        max = (eval > max ? eval : max);
                        alpha = (alpha > eval ? alpha : eval);
                        if (beta <= alpha)
                            i = 100;
                        std::cout<< "return max: " << max <<" eval after: " << eval << std::endl;
                        break;
                case 5:eval = white_R_Q_B( board, i, dir_Q_Ki, depth, alpha, beta );
                        max = (eval > max ? eval : max);
                        alpha = (alpha > eval ? alpha : eval);
                        if (beta <= alpha)
                            i = 100;
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
        int min = INT_MAX; //static ?
        
        for (int i = 20; i < 100; i++) {

            switch (board[i]) {
                /*case 11:eval = black_P( board,i, dir_bP, depth -1);
                        min = (eval > min ? eval : min);
                        break;*/
                case 12:eval = black_Kn( board, i, dir_Kn, depth, alpha, beta );
                        min = (eval < min ? eval : min);
                        beta = (beta < eval ? beta : eval);
                        if( beta <= alpha)
                            i = 100;
                        break;
                case 13:eval = black_R_Q_B( board, i, dir_B, depth, alpha, beta );
                        min = (eval < min ? eval : min);
                        beta = (beta < eval ? beta : eval);
                        if( beta <= alpha)
                            i = 100;
                        break;
                case 14:eval = black_R_Q_B( board, i, dir_R, depth, alpha, beta );
                        min = (eval < min ? eval : min);
                        beta = (beta < eval ? beta : eval);
                        if( beta <= alpha)
                            i = 100;
                        break;
                case 15:eval = black_R_Q_B( board, i, dir_Q_Ki, depth, alpha, beta );
                        min = (eval < min ? eval : min);
                        beta = (beta < eval ? beta : eval);
                        if( beta <= alpha)
                            i = 100;
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
    
int white_R_Q_B(int board[], int pos, int dir[], int depth, int alpha, int beta)
{
    int copy_pos = pos; 
    int copy_piece = 0, eval, max = INT_MIN;
    for(int y = 1; y <= dir[0]; y++) { // check every direction the piece can go
        while(board[copy_pos+dir[y]] == 0 || board[copy_pos+dir[y]] > 10) { // move or capture
            if (board[copy_pos+dir[y]] > 10) { // capture
                change_pos(board, (copy_pos+dir[y]), pos, &copy_piece);

                eval = move_gen(board, depth - 1, alpha, beta, false); //after white move -> black move
                max = (eval > max ? eval : max);
                alpha = (alpha > eval ? alpha : eval);
                if (beta <= alpha)
                    break;

                change_pos_back(board, (copy_pos+dir[y]),pos , copy_piece);
                break; // Stop, because a piece is captured
            } 
            change_pos(board, copy_pos+dir[y], pos, &copy_piece);

            eval = move_gen(board, depth -1, alpha, beta, false);
            max = (eval > max ? eval : max);
            alpha = (alpha > eval ? alpha : eval);
            if (beta <= alpha)
                break;

            change_pos_back(board, copy_pos+dir[y], pos, copy_piece);
            copy_pos += dir[y];
        }
        //after first direction is done, the copy pos needs to be resettet
        copy_pos=pos;
    }
    return (max);
}

int white_Kn(int board[], int pos, int dir[], int depth, int alpha, int beta) {
    int copy_piece, eval, max = INT_MIN;
    for(int y = 1; y <= dir[0]; y++) {//first position of "dir" gives amount of directions
        if(board[pos+dir[y]] == 0 || board[pos+dir[y]] > 10)  {// move or capture
            change_pos(board, pos+dir[y], pos, &copy_piece);

            eval = move_gen(board, depth-1, alpha, beta, false); //after white move -> black move
            max = (eval > max ? eval : max);
            alpha = (alpha > eval ? alpha : eval);
            if (beta <= alpha)
                break;

            change_pos_back(board, pos+dir[y], pos, copy_piece);
        }
    }
    return (max);
}/*
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

/*
void white_P(Position  int pos, int dir[], int depth) { // en passant fehlt noch
    int copy_piece=0;
    if(board[pos+dir[1]] == 0) { //1 Forward
        if(pos+dir[3] < 30) // promotion
        {
            board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                board[pos]=i;
                if(depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            board[pos+dir[1]]=0;
            board[pos]=1;
        }
        else
        {
            board[pos+dir[1]]=board[pos]; //move 1
            board[pos]=0;
            if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
            board[pos]=board[pos+dir[1]]; // Returns moved Piece to old Position
            board[pos+dir[1]]=0;
        }

    }
    if(board[pos+dir[1]]==0 && board[pos+dir[2]==0] && pos > 80)//2 Forward if Pawn is still on the 2nd Rank
    {
        board[pos+dir[2]]=board[pos]; // move 2
        board[pos]=0;
        if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
        board[pos]=board[pos+dir[2]]; // Returns moved Piece to old Position
        board[pos+dir[2]]=0;
    }
    if(board[pos+dir[3]] > 10) // capture
    {
        if(pos+dir[3] < 30) // promotion
        {
            copy_piece=board[pos+dir[3]];
            board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                board[pos]=i;
                if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            board[pos+dir[3]]=copy_piece;
        }else
        {
            board[pos+dir[3]]=board[pos];
            board[pos]=0;
            if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
            board[pos]=board[pos+dir[3]]; // Returns moved Piece to old Position
            board[pos+dir[3]]=0;
        }

    }
    if(board[pos+dir[4]] > 10) // capture
    {

        if(pos+dir[4] < 30) // promotion
        {
            copy_piece=board[pos+dir[4]];
            board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                board[pos]=i;
                if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            board[pos+dir[4]]=copy_piece;
        }
        else
        {
            board[pos+dir[4]]=board[pos];
            board[pos]=0;
            if(depths<MAXDEPTHS)
                {
                    move_gen(;
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<"ERROR"<<std::endl;
            board[pos]=board[pos+dir[4]]; // Returns moved Piece to old Position
            board[pos+dir[4]]=0;
        }
    }
}*/

/** black pieces **/

int black_R_Q_B(int board[], int pos, int dir[], int depth, int alpha, int beta) {
    int copy_pos = pos;
    int copy_piece, eval, min = INT_MAX;
    for(int y = 1; y <= dir[0]; y++) {
        while(board[copy_pos+dir[y]] == 0 || (board[copy_pos+dir[y]] < 10 && board[copy_pos+dir[y]] > 0)) { // move or capture
            if (board[copy_pos+dir[y]] < 10 && board[copy_pos+dir[y]] > 0) {
                change_pos(board, (copy_pos+dir[y]), pos, &copy_piece);

                eval = move_gen(board, depth -1, alpha, beta, true);
                min = (eval < min ? eval : min);
                beta = (beta < eval ? beta : eval);
                if( beta <= alpha)
                    break;

                change_pos_back(board, (copy_pos+dir[y]),pos , copy_piece);
                break;//If Piece is captured, it cant move further
            }
            change_pos(board, (copy_pos+dir[y]), pos, &copy_piece);

            eval = move_gen(board, depth -1, alpha, beta, true);
            min = (eval < min ? eval : min);
            beta = (beta < eval ? beta : eval);
            if( beta <= alpha)
                break;

            change_pos_back(board, (copy_pos+dir[y]),pos , copy_piece);

            copy_pos += dir[y];
        }
        copy_pos = pos;
    }
    return (min);
}
int black_Kn(int board[], int pos ,int dir[], int depth, int alpha, int beta)
{
    int copy_piece, eval, min = INT_MAX;
    for(int y = 1; y <= dir[0]; y++) {//first index of "dir" gives amount of directions
        if(board[pos+dir[y]] == 0 || (board[pos+dir[y]] < 10 && board[pos+dir[y]] > 0)) {// move or capture
/*
            copy_piece = board[pos+dir[y]]; //Saves captured Piece or empty square
            board[pos+dir[y]] = board[pos];
            board[pos] = 0;
*/
            change_pos(board, pos+dir[y], pos, &copy_piece);

            eval = move_gen(board, depth -1, alpha, beta, true);
            min = (eval < min ? eval : min);
            beta = (beta < eval ? beta : eval);
            if( beta <= alpha)
                break;

            change_pos_back(board, pos+dir[y], pos, copy_piece);

/*
            board[pos] = board[pos+dir[y]]; // Returns moved Piece to old Position
            board[pos+dir[y]] = copy_piece; // Returns captured Piece or empty square
*/
        }
    }
    return (min);
}
