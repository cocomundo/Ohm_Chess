#include <stdlib.h>
//#include "move_generator.h"
#include "position.h"
#include <iostream>
#include <iomanip>

/** some thoughts**/
    /**generates 20 different positions at the beginning -> calles internal the eval function for all positions -> calls move_gen and gives him the current position,
    the value of the position and the depth of the recursion
    good: we can control the calculation, we could use the minimax algorithm(have heard of it)
    bad: we check every position which means insane computing load -> alpha/beta pruning which disregards bad moves, therefore it takes less computing load**/

    /**hardcore: hashing positions --> same position same hash**/
    /** some general information https://www.quora.com/Which-algorithms-are-used-in-chess-engines**/
    /** https://www.freecodecamp.org/news/simple-chess-ai-step-by-step-1d55a9266977/ **/

int main()
{
    Position game;
    game.move_gen(board[]);
    return 0;
}
