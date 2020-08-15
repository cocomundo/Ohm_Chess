#include "move_gen.h"
#include "board.h"
#include <iostream>
#include <time.h>
#include <limits.h> // INT_MIN, INT_MAX


#define MAXDEPTH 7

int main()
{
    int eval = 0;
    clock_t begin = clock();

    /* start the game state machine */
    // ...
    /* start move generator*/
    eval = move_gen(board, MAXDEPTH, INT_MIN, INT_MAX, true);
    std::cout << "Endresult: " << eval << std::endl;

    /* calculate the needed time */
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Needed Time: %lf\n", time_spent);


/**
    Player player1, player2;
    Chess_board  board;
    player1.move_gen;
    player2.move_gen;
*/
    return 0;
}
