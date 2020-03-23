#ifndef POSITION_H
#define POSITION_H


class Position
{
    public:
        Position();
        ~Position();
        bool b_w=0; // move of black=1 or white=0
        bool short_castle_b=1; // is short_castle still allowed
        bool long_castle_b=1; // is short_castle still allowed
        bool short_castle_w=1; // is short_castle still allowed
        bool long_castle_w=1; // is short_castle still allowed
        int pos_eval=0;
        void get_new_pos(int board[]);
        Position *p_next_pos[200]; // max. possible move count

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
********************************************/
        int board[120] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
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
