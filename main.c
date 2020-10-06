#include "move_gen.h"
#include "board.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <limits.h>



int main()
{
    int eval = 0; /* stores the evaluation value of the best move */
    int ret = 0; /* dummy var for return value checks */
    bool color_to_move; /* stores the current color to move (toggles each move) */
    bool color_player; /* stores the color of the player playing against the engine*/
    int game_state = 1; /* stores the current gamestate, 1 tells that the game is runable */
    /* 
     * typedef structure stores:
     * the start square of the current best move
     * the end square of the current best move
     * information if and what kind of special move
     */
    MOVE best_move;
    /* time vars for benchmarking */
    clock_t start, end;
    double cpu_time_used;


    /* Init curses */
    ret = init_curses();
    if(ret == EXIT_FAILURE){
        return 0;
    }

    /* start the game state machine */
    color_player = game_start();

    /* white always starts the game */
    color_to_move = true;

    /* draw init board */
    refresh_board(board);

    /* if the player chooses to play black */
    if(color_player == true){
        start = clock();
        /* engine is called */
        eval = move_gen(board, MAXDEPTH, INT_MIN, INT_MAX, color_to_move, false,
            long_castle_white, long_castle_black, short_castle_white,
            short_castle_black, &best_move);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        /* move is saved on the board and global rules are checked */
        make_move(best_move.start_pos, best_move.end_pos, best_move.special,
            &move_50_rule, &color_to_move, &game_state);
        /* new position is updated in ncurses */
        refresh_board(board);
        /* prints the engine-move and evaluation value */
        show_move(best_move.start_pos, best_move.end_pos, best_move.special, eval, cpu_time_used);
    }

    while(game_state == 1){
        /* Player */
        get_move_user(&best_move.start_pos, &best_move.end_pos, &best_move.special);
        /* move is saved on the board and global rules are checked */
        make_move(best_move.start_pos, best_move.end_pos, best_move.special,
            &move_50_rule, &color_to_move, &game_state);
        /* new position is updated in ncurses */
        refresh_board(board);
        
        if(game_state != 1)
            break;

        start = clock();
        /* Engine*/
        eval = move_gen(board, MAXDEPTH, INT_MIN, INT_MAX, color_to_move, false,
            long_castle_white, long_castle_black, short_castle_white,
            short_castle_black, &best_move);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        /* move is saved on the board and global rules are checked */
        make_move(best_move.start_pos, best_move.end_pos, best_move.special,
            &move_50_rule, &color_to_move, &game_state);
        /* new position is updated in ncurses */
        refresh_board(board);
        /* prints the engine-move and evaluation value */
        show_move(best_move.start_pos, best_move.end_pos, best_move.special, eval, cpu_time_used);
    }

    /* print how the game exited */
    print_game_exit(game_state, color_player);
    
    return 0;
}
