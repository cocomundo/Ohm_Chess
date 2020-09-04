#include "move_gen.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* INT_MIN, INT_MAX */
#include <limits.h>
#include <curses.h>
#include <unistd.h>

#define MAXDEPTH 2
#define KEY_Y 121

void quit(){
  endwin();
}

int main()
{
    int input_char = 0, eval = 0;
    clock_t begin = clock();

    /* start the game state machine */

    /* curses initialisieren */
    initscr ();
    /* enable key-strokes*/
    keypad (stdscr, TRUE);
    /* disable print or pressed key*/
    noecho();

    atexit(quit);
    curs_set(0);

    mvaddstr(3, 12, "***Chess Engine - Welcome***");
    mvaddstr(5, 2, "Do you think you can beat the engine ?");
    mvaddstr(6, 2, "Start the Game with \"y\"!");
    mvaddstr(7, 2, "To quit the Game press \"q\" or any other key");
    if((input_char = getch ()) == 'y'){
        clear();
        mvaddstr(5, 2, "Good Luck !");
        refresh();
        sleep(2);
        mvaddstr(5, 2, "Game content");
        refresh();
    }else{
        mvaddstr(5, 2, "Game content");
        refresh();
        sleep(2);
        return 0;
    }
    
    refresh();
    getch();
    return 0;

    /* start move generator*/
    eval = move_gen(board, MAXDEPTH, INT_MIN, INT_MAX, true);
    printf("endresult: %d\n", eval);

    /* calculate the needed time */
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Needed Time: %lf\n", time_spent);

    return 0;
}
