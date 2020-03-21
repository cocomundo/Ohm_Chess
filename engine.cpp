//This is our first try to programm a chess engine
#include <stdlib.h>
#include "functions.h"
#include <iostream>
#include <iomanip>


main()
{
    bool _to_move =0; // 0 = white, 1 = black
    bool *p_to_move = 0;
    p_to_move=&_to_move;
    /*
    12/10 Setting
    -1 = illegal Square
    0 = empty Square
    <10 White Pice
    >10 Black Pice
    1 = Pawn
    2 = Knight
    3 = Bishop
    4 = Rook
    5 = Queen
    6 = King
    */
    int _board[120] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
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

    gamestart(p_to_move, _board);
    move_f(p_to_move,_board);

}
