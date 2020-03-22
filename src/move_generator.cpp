#include "move_generator.h"
#include "position.h"
#include <iomanip>
#include <iostream>

void move_gen(Position position) //calculate further possible moves
//bool *p_to_move,int board[]
{
    int dir_B[5]={4,-9,9,-11,11};
    int dir_R[5]={4,-1,1,-10,10};
    int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
    int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
    int dir_wP[5]={4,-10,-20,-9,-11};
    if (position.b_w == 1) //if white is to move or black
    {
        for(int i=20; i < 100; i++)
        {
            switch(position.board[i])
            {

                case 1:white_P(position.board, i, dir_wP);
                        break;
                case 2:white_Kn(position.board, i, dir_Kn);//Knight and King have same move-function but different directions
                        break;
                case 3:white_R_Q_B(position.board, i, dir_B); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 4:white_R_Q_B(position.board, i, dir_R); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 5:white_R_Q_B(position.board, i, dir_Q_Ki); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 6:white_Ki(position.board, position.long_castle_w, position.short_castle_w, i, dir_Q_Ki);//Knight and King have same move-function but different directions
                        break;
                default:break;
            }
        }
    }/*
    else()
    {
        for(int i=20; i < 100; i++)
        {
            switch(board[i])
            {
                case 11:schwarzerbauer(i);break;
                case 12:schwarzerspringer(i);ibreak;
                case 13:schwarzerlaufer(i);break;
                case 14:schwarzerturm(i);break;
                case 15:schwarzedame(i);break;
                case 16:schwarzerkoenig(i);break;
                default;break;
            }

        }

    }*/
    //best_move=

}
void white_R_Q_B(int board[],int pos, int dir[])
{
    int copy_pos=pos;
    int fig;
    for(int y=1; y<=dir[0]; y++)
    {
        while(board[copy_pos+dir[y]] == 0 || board[copy_pos+dir[y]]> 10) // move or capture
        {
            if (board[copy_pos+dir[y]]> 10);
            {
                fig=board[copy_pos+dir[y]];
                board[copy_pos+dir[y]]=board[pos];
                board[pos]=0;
                pos_eval(board);
                board[pos]=board[copy_pos+dir[y]];
                board[copy_pos+dir[y]]=fig;
                break;//If Piece is captured cant move further
            }

            board[copy_pos+dir[y]]=board[pos];
            board[pos]=0;
            pos_eval(board);
            board[pos]=board[copy_pos+dir[y]];
            board[copy_pos+dir[y]]=fig;
            copy_pos+=dir[y];
        }
        copy_pos=pos;
    }
}
void white_Kn(int board[], int pos,int dir[])
{
    int fig;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(board[pos+dir[y]] == 0 || board[pos+dir[y]] > 10) // move or capture
        {
            fig=board[pos+dir[y]]; //Saves captured Piece or empty square
            board[pos+dir[y]]=board[pos];
            board[pos]=0;
            pos_eval(board); // Evaluation of the Position
            board[pos]=board[pos+dir[y]]; // Returns moved Piece to old Position
            board[pos+dir[y]]=fig; // Returns captured Piece or empty square
        }
    }
}
void white_Ki(Position position,int board[],bool long_castle_w, bool short_castle_w, int pos,int dir[])
{
    int fig;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(board[pos+dir[y]] == 0 || board[pos+dir[y]] > 10) // move or capture
        {
            fig=board[pos+dir[y]]; //Saves captured Piece or empty square
            board[pos+dir[y]]=board[pos];
            board[pos]=0;
            pos_eval(board); // Evaluation of the Position
            board[pos]=board[pos+dir[y]]; // Returns moved Piece to old Position
            board[pos+dir[y]]=fig; // Returns captured Piece or empty square
        }
    }
    if(long_castle_w==1&&pos==95&&board[94]==0&&board[93]==0&&board[92]==0&&board[91]==4)
    {
        position
        board[91]=0;
        board[93]=6;
        board[94]=4;
        board[95]=0;
        pos_eval();
        position.get_new_pos(board);
        board[91]=4;
        board[93]=0;
        board[94]=0;
        board[95]=6;
    }

}
void white_P(int board[], int pos, int dir[]) ///en passant fehlt noch
{
    int fig=0;
    if(board[pos+dir[1]]==0)//1 Forward
    {
        if(pos+dir[3] < 30) // promotion
        {
            board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                board[pos]=i;
                pos_eval(board);
            }
            board[pos+dir[1]]=0;
        }
        else
        {
            board[pos+dir[1]]=board[pos]; //move 1
            board[pos]=0;
            pos_eval(board); // Evaluation of the Position
            board[pos]=board[pos+dir[1]]; // Returns moved Piece to old Position
            board[pos+dir[1]]=0;
        }

    }
    if(board[pos+dir[1]]==0 && board[pos+dir[2]] && pos > 80)//2 Forward if Pawn is still on the 2nd Rank
    {
        board[pos+dir[2]]=board[pos]; // move 2
        board[pos]=0;
        pos_eval(board); // Evaluation of the Position
        board[pos]=board[pos+dir[2]]; // Returns moved Piece to old Position
        board[pos+dir[2]]=0;
    }
    if(board[pos+dir[3]] > 10) // capture
    {
        if(pos+dir[3] < 30) // promotion
        {
            fig=board[pos+dir[3]];
            board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                board[pos]=i;
                pos_eval(board);
            }
            board[pos+dir[3]]=fig;
        }else
        {
            board[pos+dir[3]]=board[pos];
            board[pos]=0;
            pos_eval(board); // Evaluation of the Position
            board[pos]=board[pos+dir[3]]; // Returns moved Piece to old Position
            board[pos+dir[3]]=0;
        }

    }
    if(board[pos+dir[4]] > 10) // capture
    {

        if(pos+dir[4] < 30) // promotion
        {
            fig=board[pos+dir[4]];
            board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                board[pos]=i;
                pos_eval(board);
            }
            board[pos+dir[4]]=fig;
        }
        else
        {
            board[pos+dir[4]]=board[pos];
            board[pos]=0;
            pos_eval(board); // Evaluation of the Position
            board[pos]=board[pos+dir[4]]; // Returns moved Piece to old Position
            board[pos+dir[4]]=0;
        }
    }
}
/*
void test_move(int board[],int pos,int new_pos)
{
    int fig=board[new_pos];
    board[new_pos]=board[pos];
    board[pos]=0;
    pos_eval(board); // Evaluation of the Position
    board[pos]=board[new_pos]; // Returns moved Piece to old Position
    board[new_pos]]=0;
    board[new_pos]=fig;
}*/

void pos_eval(int board[])
{
    int evaluation=0;
    //if(pos is good)
    for(int i=20;i<100;i++)
    {
        switch(board[i])
        {
            case 1: evaluation+=100;break;
            case 11: evaluation-=100;break;
            case 2: evaluation+=280;break;
            case 3: evaluation+=300;break;
            case 4: evaluation+=500;break;
            case 5: evaluation+=900;break;
            case 6: evaluation+=10000;break;
            case 12: evaluation-=280;break;
            case 13: evaluation-=300;break;
            case 14: evaluation-=500;break;
            case 15: evaluation-=900;break;
            case 16: evaluation-=10000;break;
            default: break;
        }
    }
    std::cout<<evaluation<<std::endl;

    //best_move()


}
/*
 for (int i=0 ; i<MAX_FIELD ; i++)
    {
        board[i]=0;

        if(i%10==0)
        {
            std::cout <<std::endl;
        }
        std::cout << std::setw(5)<<board[i];
    }
    std::cout<<std::endl;
*/