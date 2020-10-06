#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdbool.h>

#define OFFSET_X 10
#define OFFSET_Y 6
#define KEY_Y 121

#define MAX_CHAR_MOVE 2

#define BLUE_BLACK_P 1
#define BLUE_WHITE_P 2
#define YELLOW_BLACK_P 3
#define YELLOW_WHITE_P 4
#define WHITE_P 5
#define BLACK_P 6
#define RED_P 7

void get_move_user(int *start_pos, int *end_pos, int *special_move);
void create_board_large(void);
void create_board_small(void);
void refresh_board(int board[]);
void show_move(int start_pos, int end_pos, int special, int eval, double time);
void print_game_exit(int game_state, bool w_b);

int init_curses(void);
bool game_start(void);
void game_cycle();
void quit(void);


#endif
