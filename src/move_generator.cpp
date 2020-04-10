#include "move_generator.h"
#include "position.h"

#include <iomanip>
#include <iostream>
#define MAXDEPTHS 5
void move_gen(Position position, Direction directions) //calculate further possible moves
//bool *p_to_move,int board[]
{
    position.depths++;
    printf("Tiefe: %d\n", depths);
    /*
    int dir_B[5]={4,-9,9,-11,11};
    int dir_R[5]={4,-1,1,-10,10};
    int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
    int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
    int dir_wP[5]={4,-10,-20,-9,-11};
    int dir_bP[5]={4,10,20,9,11};
    */

    if(position.b_w==0)
    {
        for(int i=20; i < 100; i++)
        {
            std::cout << "hier sind wir"<< i<<std::endl;


            switch(position.board[i])
            {

                case 1:white_P(position.board, i, directions.dir_wP());
                        break;
                case 2:white_Kn(position.board, i, directions.dir_Kn());//Knight and King have same move-function but different directions
                        break;
                case 3:white_R_Q_B(position.board, i, directions.dir_B()); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 4:white_R_Q_B(position.board, i, directions.dir_R()); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 5:white_R_Q_B(position.board, i, directions.dir_Q_Ki()); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 6:white_Ki(position.board, position.long_castle_w, position.short_castle_w, i, directions.dir_Q_Ki());//Knight and King have same move-function but different directions
                        break;
                default:break;
            }
        }

    }else{
        for(int i=20; i < 100; i++)
        {
            std::cout << "hier sind wir"<< i<<std::endl;


            switch(position.board[i])
            {
                case 11:black_P(position.board, i, directions.dir_bP());
                        break;
                case 12:black_Kn(position.board, i, directions.dir_Kn());
                        break;
                case 13:black_R_Q_B(position.board, i, directions.dir_B());
                        break;
                case 14:black_R_Q_B(position.board, i, directions.dir_R());
                        break;
                case 15:black_R_Q_B(position.board, i, directions.dir_Q_Ki());
                        break;
                case 16:black_Ki(position.board, position.long_castle_b, position.short_castle_b, i, directions.dir_Q_Ki());
                        break;
                default:break;
            }
        }

    }
}
/*
bool chess_check(int board[], bool move_b_w, Direction directions)
{
    int i=0, j=0, k=0;;
    int king_pos=0;
    bool check;
    if (move_b_w) // true == 1  1==black -> check white moves
    {
        for(i=20;i<100;i++)
        {
            if (board[i]==16)//black king
            {
                king_pos=i;
                for(j=1; j<=directions.dir_Kn()[0];j++)
                {
                    if(king_pos+directions.dir_Kn()[j] == 2)
                    {
                        return (check = 1);
                    }
                }
                if(king_pos+9==1 || king_pos+11=1)
                {
                    return (check = 1);
                }
                m_dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
                for(j=1;j<=directions.dir_Q_Ki()[0];j++)
                {
                    for(k=0;k<7;k++)
                    {
                        if(king_pos+dir_Q_Ki[k]==)

                    }
                }
            }
        }



    }
    else // false == 0, 0==white -> check black moves
    {

    }
}
*/
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
                if(depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(depths==MAXDEPTHS)
                    pos_eval(board);
                else
                    std::cout<<ERROR<<endl;

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
void white_Ki(int board[],bool long_castle_w, bool short_castle_w, int pos,int dir[])
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

        board[91]=0;
        board[93]=6;
        board[94]=4;
        board[95]=0;
        pos_eval(board);
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
        pos_eval(board);
        board[95]=6;
        board[96]=0;
        board[97]=0;
        board[98]=4;
    }

}
void white_P(int board[], int pos, int dir[]) // en passant fehlt noch
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
            board[pos]=1;
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
    if(board[pos+dir[1]]==0 && board[pos+dir[2]==0] && pos > 80)//2 Forward if Pawn is still on the 2nd Rank
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

/********************* black Pieces ***************/
void black_R_Q_B(int board[],int pos, int dir[])
{
    int copy_pos=pos;
    int fig;
    for(int y=1; y<=dir[0]; y++)
    {
        while(board[copy_pos+dir[y]] == 0 || (board[copy_pos+dir[y]]< 10&&board[copy_pos+dir[y]]>0)) // move or capture
        {
            if (board[copy_pos+dir[y]]< 10&&board[copy_pos+dir[y]]>0);
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
void black_Kn(int board[], int pos,int dir[])
{
    int fig;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(board[pos+dir[y]] == 0 || (board[pos+dir[y]]< 10&&board[pos+dir[y]]>0)) // move or capture
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
void black_Ki(int board[],bool long_castle_b, bool short_castle_b, int pos,int dir[])
{
    int fig;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(board[pos+dir[y]] == 0 || (board[pos+dir[y]]< 10&&board[pos+dir[y]]>0)) // move or capture
        {
            fig=board[pos+dir[y]]; //Saves captured Piece or empty square
            board[pos+dir[y]]=board[pos];
            board[pos]=0;
            pos_eval(board); // Evaluation of the Position
            board[pos]=board[pos+dir[y]]; // Returns moved Piece to old Position
            board[pos+dir[y]]=fig; // Returns captured Piece or empty square
        }

    }
    if(long_castle_b==1&&pos==25&&board[24]==0&&board[23]==0&&board[22]==0&&board[21]==14)
    {
        board[21]=0;
        board[23]=16;
        board[24]=14;
        board[25]=0;
        pos_eval(board);
        board[21]=14;
        board[23]=0;
        board[24]=0;
        board[25]=16;
    }
    if(short_castle_b==1&&pos==25&&board[26]==0&&board[27]==0&&board[28]==14)
    {

        board[25]=0;
        board[26]=14;
        board[27]=16;
        board[28]=0;
        pos_eval(board);
        board[25]=16;
        board[26]=0;
        board[27]=0;
        board[28]=14;
    }

}
void black_P(int board[], int pos, int dir[]) // en passant fehlt noch
{
    int fig=0;
    if(board[pos+dir[1]]==0)//1 Forward
    {
        if(pos+dir[3] > 90) // promotion
        {
            board[pos]=0;
            for(int i=12;i<=15;i++)
            {
                board[pos]=i;
                pos_eval(board);
            }
            board[pos+dir[1]]=0;
            board[pos]=11;
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
    if(board[pos+dir[1]]==0 && board[pos+dir[2]]==0 && pos < 40)//2 Forward if Pawn is still on the 2nd Rank
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
    std::cout << "fig gezogen"<< std::endl;
    int evaluation=0;
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
