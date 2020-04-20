#include <stdlib.h>
#include "move_generator.h"
#include "position.h"
#include <iostream>
#include <iomanip>


int main()
{
    int depth=0;
    Direction directions;
    Position positionstart;
    move_gen(positionstart, depth);
    return 0;
}
