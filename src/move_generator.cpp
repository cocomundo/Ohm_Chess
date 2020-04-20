/*
#include "move_generator.h"
#include "position.h"

#include <iomanip>
#include <iostream>
#define MAXDEPTHS 5
void move_gen(Position position, int depth) //calculate further possible moves
//bool *p_to_move,int position.board[]
{
    printf("Tiefe: %d\n",depth);

    int dir_B[5]={4,-9,9,-11,11};
    int dir_R[5]={4,-1,1,-10,10};
    int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
    int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
    int dir_wP[5]={4,-10,-20,-9,-11};
    int dir_bP[5]={4,10,20,9,11};


    if(position.b_w==0)
    {
        for(int i=20; i < 100; i++)
        {
            std::cout << "hier sind wir"<< i<<std::endl;


            switch(position.board[i])
            {

                case 1:white_P(position, i, dir_wP, depth);
                        break;
                case 2:white_Kn(position, i, dir_Kn, depth);//Knight and King have same move-function but different directions
                        break;
                case 3:white_R_Q_B(position, i, dir_B, depth); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 4:white_R_Q_B(position, i, dir_R, depth); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 5:white_R_Q_B(position, i, dir_Q_Ki, depth); //Rook, Queen and Bishop have same move-function but different directions
                        break;
                case 6:white_Ki(position, position.long_castle_w, position.short_castle_w, i, dir_Q_Ki, depth);//Knight and King have same move-function but different directions
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
                case 11:black_P(position, i, dir_bP, depth);
                        break;
                case 12:black_Kn(position, i, dir_Kn, depth);
                        break;
                case 13:black_R_Q_B(position, i, dir_B, depth);
                        break;
                case 14:black_R_Q_B(position, i, dir_R, depth);
                        break;
                case 15:black_R_Q_B(position, i, dir_Q_Ki, depth);
                        break;
                case 16:black_Ki(position, position.long_castle_b, position.short_castle_b, i, dir_Q_Ki, depth);
                        break;
                default:break;
            }
        }

    }
}
/*
bool chess_check(int position.board[], bool move_b_w, Direction directions)
{
    int i=0, j=0, k=0;;
    int king_pos=0;
    bool check;
    if (move_b_w) // true == 1  1==black -> check white moves
    {
        for(i=20;i<100;i++)
        {
            if (position.board[i]==16)//black king
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

int m_dir_B[5]={4,-9,9,-11,11};
        int m_dir_R[5]={4,-1,1,-10,10};
        int m_dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
        int m_dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
        int m_dir_wP[5]={4,-10,-20,-9,-11};
        int m_dir_bP[5]={4,10,20,9,11};

void white_R_Q_B(Position position,int pos, int dir[], int depth)
{

    int copy_pos=pos;
    int fig;
    for(int y=1; y<=dir[0]; y++)
    {
        while(position.board[copy_pos+dir[y]] == 0 || position.board[copy_pos+dir[y]]> 10) // move or capture
        {
            if (position.board[copy_pos+dir[y]]> 10);
            {
                fig=position.board[copy_pos+dir[y]];
                position.board[copy_pos+dir[y]]=position.board[pos];
                position.board[pos]=0;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position, depth);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;

                position.board[pos]=position.board[copy_pos+dir[y]];
                position.board[copy_pos+dir[y]]=fig;
                break;//If Piece is captured cant move further
            }

            position.board[copy_pos+dir[y]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position, depth);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[copy_pos+dir[y]];
            position.board[copy_pos+dir[y]]=fig;
            copy_pos+=dir[y];
        }
        copy_pos=pos;
    }
}
void white_Kn(Position position, int pos,int dir[], int depth)
{
    int fig, eval, bestmoveeval, remembermove;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(position.board[pos+dir[y]] == 0 || position.board[pos+dir[y]] > 10) // move or capture
        {
            fig=position.board[pos+dir[y]]; //Saves captured Piece or empty square
            position.board[pos+dir[y]]=position.board[pos];
            position.board[pos]=0;
            if(depth<MAXDEPTHS)
            {
                move_gen(position, depth+1);
            }
            else if(position.depths==MAXDEPTHS)
            {
                eval=pos_eval(position.board);
                if(eval>bestmoveeval)
                {
                    bestmoveeval=eval;
                   bestmove=thisone;
                }
            }
            else
                std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[y]]; // Returns moved Piece to old Position
            position.board[pos+dir[y]]=fig; // Returns captured Piece or empty square
        }
    }
}
void white_Ki(Position position,bool long_castle_w, bool short_castle_w, int pos,int dir[], int depth)
{
    int fig;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(position.board[pos+dir[y]] == 0 || position.board[pos+dir[y]] > 10) // move or capture
        {
            fig=position.board[pos+dir[y]]; //Saves captured Piece or empty square
            position.board[pos+dir[y]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[y]]; // Returns moved Piece to old Position
            position.board[pos+dir[y]]=fig; // Returns captured Piece or empty square
        }
    }

    if(long_castle_w==1&&pos==95&&position.board[94]==0&&position.board[93]==0&&position.board[92]==0&&position.board[91]==4)
    {

        position.board[91]=0;
        position.board[93]=6;
        position.board[94]=4;
        position.board[95]=0;
        if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
        position.board[91]=4;
        position.board[93]=0;
        position.board[94]=0;
        position.board[95]=6;
    }
    if(short_castle_w==1&&pos==95&&position.board[96]==0&&position.board[97]==0&&position.board[98]==4)
    {

        position.board[95]=0;
        position.board[96]=4;
        position.board[97]=6;
        position.board[98]=0;
        if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
        position.board[95]=6;
        position.board[96]=0;
        position.board[97]=0;
        position.board[98]=4;
    }

}
void white_P(Position position, int pos, int dir[], int depth) // en passant fehlt noch
{
    int fig=0;
    if(position.board[pos+dir[1]]==0)//1 Forward
    {
        if(pos+dir[3] < 30) // promotion
        {
            position.board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                position.board[pos]=i;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            position.board[pos+dir[1]]=0;
            position.board[pos]=1;
        }
        else
        {
            position.board[pos+dir[1]]=position.board[pos]; //move 1
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[1]]; // Returns moved Piece to old Position
            position.board[pos+dir[1]]=0;
        }

    }
    if(position.board[pos+dir[1]]==0 && position.board[pos+dir[2]==0] && pos > 80)//2 Forward if Pawn is still on the 2nd Rank
    {
        position.board[pos+dir[2]]=position.board[pos]; // move 2
        position.board[pos]=0;
        if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
        position.board[pos]=position.board[pos+dir[2]]; // Returns moved Piece to old Position
        position.board[pos+dir[2]]=0;
    }
    if(position.board[pos+dir[3]] > 10) // capture
    {
        if(pos+dir[3] < 30) // promotion
        {
            fig=position.board[pos+dir[3]];
            position.board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                position.board[pos]=i;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            position.board[pos+dir[3]]=fig;
        }else
        {
            position.board[pos+dir[3]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[3]]; // Returns moved Piece to old Position
            position.board[pos+dir[3]]=0;
        }

    }
    if(position.board[pos+dir[4]] > 10) // capture
    {

        if(pos+dir[4] < 30) // promotion
        {
            fig=position.board[pos+dir[4]];
            position.board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                position.board[pos]=i;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            position.board[pos+dir[4]]=fig;
        }
        else
        {
            position.board[pos+dir[4]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[4]]; // Returns moved Piece to old Position
            position.board[pos+dir[4]]=0;
        }
    }
}

/********************* black Pieces ***************
void black_R_Q_B(Position position,int pos, int dir[], int depth)
{
    int copy_pos=pos;
    int fig;
    for(int y=1; y<=dir[0]; y++)
    {
        while(position.board[copy_pos+dir[y]] == 0 || (position.board[copy_pos+dir[y]]< 10&&position.board[copy_pos+dir[y]]>0)) // move or capture
        {
            if (position.board[copy_pos+dir[y]]< 10&&position.board[copy_pos+dir[y]]>0);
            {
                fig=position.board[copy_pos+dir[y]];
                position.board[copy_pos+dir[y]]=position.board[pos];
                position.board[pos]=0;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
                position.board[pos]=position.board[copy_pos+dir[y]];
                position.board[copy_pos+dir[y]]=fig;
                break;//If Piece is captured cant move further
            }

            position.board[copy_pos+dir[y]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[copy_pos+dir[y]];
            position.board[copy_pos+dir[y]]=fig;
            copy_pos+=dir[y];
        }
        copy_pos=pos;
    }
}
void black_Kn(Position position, int pos,int dir[], int depth)
{
    int fig;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(position.board[pos+dir[y]] == 0 || (position.board[pos+dir[y]]< 10&&position.board[pos+dir[y]]>0)) // move or capture
        {
            fig=position.board[pos+dir[y]]; //Saves captured Piece or empty square
            position.board[pos+dir[y]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[y]]; // Returns moved Piece to old Position
            position.board[pos+dir[y]]=fig; // Returns captured Piece or empty square
        }
    }
}
void black_Ki(Position position,bool long_castle_b, bool short_castle_b, int pos,int dir[], int depth)
{
    int fig;
    for(int y=1; y<=dir[0]; y++) //first position of "dir" gives amount of directions
    {
        if(position.board[pos+dir[y]] == 0 || (position.board[pos+dir[y]]< 10&&position.board[pos+dir[y]]>0)) // move or capture
        {
            fig=position.board[pos+dir[y]]; //Saves captured Piece or empty square
            position.board[pos+dir[y]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[y]]; // Returns moved Piece to old Position
            position.board[pos+dir[y]]=fig; // Returns captured Piece or empty square
        }

    }
    if(long_castle_b==1&&pos==25&&position.board[24]==0&&position.board[23]==0&&position.board[22]==0&&position.board[21]==14)
    {
        position.board[21]=0;
        position.board[23]=16;
        position.board[24]=14;
        position.board[25]=0;
        if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
        position.board[21]=14;
        position.board[23]=0;
        position.board[24]=0;
        position.board[25]=16;
    }
    if(short_castle_b==1&&pos==25&&position.board[26]==0&&position.board[27]==0&&position.board[28]==14)
    {

        position.board[25]=0;
        position.board[26]=14;
        position.board[27]=16;
        position.board[28]=0;
        if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
        position.board[25]=16;
        position.board[26]=0;
        position.board[27]=0;
        position.board[28]=14;
    }

}
void black_P(Position position, int pos, int dir[], int depth) // en passant fehlt noch
{
    int fig=0;
    if(position.board[pos+dir[1]]==0)//1 Forward
    {
        if(pos+dir[3] > 90) // promotion
        {
            position.board[pos]=0;
            for(int i=12;i<=15;i++)
            {
                position.board[pos]=i;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            position.board[pos+dir[1]]=0;
            position.board[pos]=11;
        }
        else
        {
            position.board[pos+dir[1]]=position.board[pos]; //move 1
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[1]]; // Returns moved Piece to old Position
            position.board[pos+dir[1]]=0;
        }

    }
    if(position.board[pos+dir[1]]==0 && position.board[pos+dir[2]]==0 && pos < 40)//2 Forward if Pawn is still on the 2nd Rank
    {
        position.board[pos+dir[2]]=position.board[pos]; // move 2
        position.board[pos]=0;
        if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
        position.board[pos]=position.board[pos+dir[2]]; // Returns moved Piece to old Position
        position.board[pos+dir[2]]=0;
    }
    if(position.board[pos+dir[3]] > 10) // capture
    {
        if(pos+dir[3] < 30) // promotion
        {
            fig=position.board[pos+dir[3]];
            position.board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                position.board[pos]=i;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            position.board[pos+dir[3]]=fig;
        }else
        {
            position.board[pos+dir[3]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[3]]; // Returns moved Piece to old Position
            position.board[pos+dir[3]]=0;
        }

    }
    if(position.board[pos+dir[4]] > 10) // capture
    {

        if(pos+dir[4] < 30) // promotion
        {
            fig=position.board[pos+dir[4]];
            position.board[pos]=0;
            for(int i=2;i<=5;i++)
            {
                position.board[pos]=i;
                if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            }
            position.board[pos+dir[4]]=fig;
        }
        else
        {
            position.board[pos+dir[4]]=position.board[pos];
            position.board[pos]=0;
            if(position.depths<MAXDEPTHS)
                {
                    move_gen(position);
                }
                else if(position.depths==MAXDEPTHS)
                    pos_eval(position.board);
                else
                    std::cout<<"ERROR"<<std::endl;
            position.board[pos]=position.board[pos+dir[4]]; // Returns moved Piece to old Position
            position.board[pos+dir[4]]=0;
        }
    }
}
/*
void test_move(int position.board[],int pos,int new_pos)
{
    int fig=position.board[new_pos];
    position.board[new_pos]=position.board[pos];
    position.board[pos]=0;
    pos_eval(position.board); // Evaluation of the Position
    position.board[pos]=position.board[new_pos]; // Returns moved Piece to old Position
    position.board[new_pos]]=0;
    position.board[new_pos]=fig;
}

int pos_eval(int board[])
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
return evaluation;

}
/*
 for (int i=0 ; i<MAX_FIELD ; i++)
    {
        position.board[i]=0;

        if(i%10==0)
        {
            std::cout <<std::endl;
        }
        std::cout << std::setw(5)<<position.board[i];
    }
    std::cout<<std::endl;
*/
