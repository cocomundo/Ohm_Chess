#ifndef POSITION_H
#define POSITION_H


#include <stdlib.h>
#include <iostream>
#include <iomanip>

#define MAXDEPTHS 5


class Position
{
    public:
        Position();
        ~Position();
        int depths=0;
        bool b_w=0; // move of black=1 or white=0
        bool short_castle_b=1; // is short_castle still allowed
        bool long_castle_b=1; // is short_castle still allowed
        bool short_castle_w=1; // is short_castle still allowed
        bool long_castle_w=1; // is short_castle still allowed
        //void get_new_pos(int board[]);
        //Position *p_next_pos[200]; // max. possible move count

        void move_gen();
        int pos_eval(int board[]);


        int dir_B[5]={4,-9,9,-11,11};
        int dir_R[5]={4,-1,1,-10,10};
        int dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
        int dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
        int dir_wP[5]={4,-10,-20,-9,-11};
        int dir_bP[5]={4,10,20,9,11};


        bool chess_check(bool move_b_w);
        void white_P(int pos, int dir[], int depth);
        void white_Kn(int pos, int dir[], int depth);
        void white_R_Q_B(int pos, int dir[], int depth);
        void white_Ki(bool long_c, bool short_c, int pos, int dir[], int depth);

        void black_P(int pos, int dir[], int depth);
        void black_Kn(int pos, int dir[], int depth);
        void black_R_Q_B(int pos, int dir[], int depth);
        void black_Ki(bool long_c, bool short_c, int pos, int dir[], int depth);


/******************************************
            12x10 Board Setting
            -1 = illegal Square
            0 = empty Square
            <10 White Piece
            >10 Black Piece
            1 = Pawn
            2 = Knight
            3 = Bishop
            4 = Rook
            5 = Queen
            6 = King
********************************************/int board[120] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
               -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
               -1,14,12,13,15,16,13,12,14,-1,
               -1,11,11,11,11,11,11,11,11,-1,
               -1,00,00,00,00,00,00,00,00,-1,
               -1,00,00,00,00,00,00,00,00,-1,
               -1,00,00,00,00,00,00,00,00,-1,
               -1,00,00,00,00,00,00,00,00,-1,
               -1,01,01,01,01,01,01,01,01,-1,
               -1,04,02,03,05,06,03,02,04,-1,
               -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
               -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};

/*********************************************
   eq. 14080060
   14=black_rook 080=start_pos 060=end_pos
   to get piece -> /1000000
   to get start_pos -> /1000 %1000
   to get end_pos -> %1000
**********************************************/
        int move_history[1000]={0};



    protected:

    private:
};

#endif // POSITION_H
