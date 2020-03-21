#include "functions.h"
#include <iomanip>
#include <iostream>

void gamestart(bool *p_to_move, int board[])
{
    *p_to_move = 1;
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
}
void move_f(bool *p_to_move,int board[])
{
    int dir_B[5]={4,-9,9,-11,11};
    int dir_R[5]={4,-1,1,-10,10};
    int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
    int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
    int dir_wP[5]={4,-10,-20,-9,-11};
    if (*p_to_move == 1) //if white is to move or black
    {
        for(int i=20; i < 100; i++)
        {
            switch(board[i])
            {

                case 1:white_P(board, i, dir_wP);
                        break;
                case 2:white_Kn_Ki(board, i, dir_Kn);//Knight and King have same move-function but different directions
                        break;
                case 3:white_R_Q_B(board, i, dir_B); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 4:white_R_Q_B(board, i, dir_R); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 5:white_R_Q_B(board, i, dir_Q_Ki); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 6:white_Kn_Ki(board, i, dir_Q_Ki);//Knight and King have same move-function but different directions
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
void white_Kn_Ki(int board[], int pos,int dir[])
{
    int fig;
    int bl_o_wh;
    if(board[pos]<10){bl_o_wh = 1;} // nachdenken!!!
    else{bl_o_wh = 11;}

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

void pos_eval(int board[])
{
    if(pos is good)

}
