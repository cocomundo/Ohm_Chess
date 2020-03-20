#include "function.h"
#include <iomanip>
#include <iostream>

void gamestart(int to_move, int board[])
{
    to_move = 0;
    for (int i=0 ; i<120 ; i++)
    {

        if(i%10==0)
        {
            std::cout <<std::endl;
        }
        std::cout << std::setw(5)<<board[i];
    }
}
