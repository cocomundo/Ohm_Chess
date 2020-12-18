#include "game.h"
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

#include <curses.h>
#include "move_gen.h"
void quit()
{
  endwin();
}

void show_move(int start_pos, int end_pos, int special, int eval, double time)
{
    double eval_double = (double) eval / 100;
    mvprintw(39, 12, "The engine moved from %d to %d and rated it %2.2lf (special move : %d)", start_pos, end_pos, eval_double, special);
    #ifdef BENCHMARK
    mvprintw(40, 12, "The calculation took: %.2lf sec", time);
    mvprintw(41, 12, "the movegenerator analyzed %2.0lf positions (%.1lf knodes/s) at a depth of %d", count,count/1000/time ,MAXDEPTH);
    #endif
    refresh();
}

void get_move_user(int *start_pos, int *end_pos, int *special_move)
{
    char move_start[MAX_CHAR_MOVE];
    char move_end[MAX_CHAR_MOVE];
    char move_special_string[MAX_CHAR_MOVE];
    int move_special = -1;
    char row_start = 'Z', row_end = 'Z';
    int column_start = -1, column_end = -1; 
    int start_square = 0, end_square = 0;

    echo();

    mvaddstr(32, 12, "Please enter the number of the special move: ");
    getnstr (move_special_string, MAX_CHAR_MOVE);
    while((sscanf(move_special_string,"%d", &move_special) != 1) 
            || move_special < 0 || move_special > 9){
        mvaddstr(32, 12, "Please enter the one of the displayed special moves: ");
        getnstr (move_special_string, MAX_CHAR_MOVE);
    }
    mvaddstr(33, 12, "Please enter the start square of your move: ");
    getnstr (move_start, MAX_CHAR_MOVE);
    while((sscanf(move_start,"%c%d", &row_start, &column_start) != 2) && column_start < 8 ){
        mvaddstr(33, 12, "Please enter the start square of your move: ");
        getnstr (move_start, MAX_CHAR_MOVE);
    }

    mvaddstr(34, 12, "Please enter the end square of your move: ");
    getnstr (move_end, MAX_CHAR_MOVE);
    while((sscanf(move_end,"%c%d", &row_end, &column_end) != 2) && column_end < 8 ){
        mvaddstr(34, 12, "Please enter the end square of your move: ");
        getnstr (move_end, MAX_CHAR_MOVE);
    }
    
    switch(row_start){
        case 'a':
            start_square += 1;
            break;
        case 'b':
            start_square += 2;
            break;
        case 'c':
            start_square += 3;
            break;
        case 'd':
            start_square += 4;
            break;
        case 'e':
            start_square += 5;
            break;
        case 'f':
            start_square += 6;
            break;
        case 'g':
            start_square += 7;
            break;
        case 'h':
            start_square += 8;
            break;
        default:
            break;
    }

    switch(column_start){
        case 1:
            start_square += 90;
            break;
        case 2:
            start_square += 80;
            break;
        case 3:
            start_square += 70;
            break;
        case 4:
            start_square += 60;
            break;
        case 5:
            start_square += 50;
            break;
        case 6:
            start_square += 40;
            break;
        case 7:
            start_square += 30;
            break;
        case 8:
            start_square += 20;
            break;
        default:
            break;
    }
    switch(row_end){
        case 'a':
            end_square += 1;
            break;
        case 'b':
            end_square += 2;
            break;
        case 'c':
            end_square += 3;
            break;
        case 'd':
            end_square += 4;
            break;
        case 'e':
            end_square += 5;
            break;
        case 'f':
            end_square += 6;
            break;
        case 'g':
            end_square += 7;
            break;
        case 'h':
            end_square += 8;
            break;
        default:
            break;
    }

    switch(column_end){
        case 1:
            end_square += 90;
            break;
        case 2:
            end_square += 80;
            break;
        case 3:
            end_square += 70;
            break;
        case 4:
            end_square += 60;
            break;
        case 5:
            end_square += 50;
            break;
        case 6:
            end_square += 40;
            break;
        case 7:
            end_square += 30;
            break;
        case 8:
            end_square += 20;
            break;
        default:
            break;
    }
    *start_pos = start_square;
    *end_pos = end_square;
    *special_move = move_special;
    
    noecho();
    clear();
    refresh();
}

void refresh_board(int board[])
{
    create_board_large();
    /* y-axis */
    for(int i = 0; i<8; i++){
        /* x-axis*/
        for(int j = 0; j<8; j++){
            switch(board[21+10*i+j]){
                case 0: /* empty square */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, ' ');
                            attroff(COLOR_PAIR(WHITE_P));
                        }else{
                            attron(COLOR_PAIR(BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, ' ');
                            attroff(COLOR_PAIR(BLACK_P));
                        }
                        break;
                case 1: /* white pawn */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(YELLOW_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'P');
                            attroff(COLOR_PAIR(YELLOW_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(YELLOW_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'P');
                            attroff(COLOR_PAIR(YELLOW_BLACK_P));
                        }
                        break;
                case 2: /* white knight */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(YELLOW_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'N');
                            attroff(COLOR_PAIR(YELLOW_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(YELLOW_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'N');
                            attroff(COLOR_PAIR(YELLOW_BLACK_P));
                        }
                        break;
                case 3: /* white bishop */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(YELLOW_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'B');
                            attroff(COLOR_PAIR(YELLOW_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(YELLOW_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'B');
                            attroff(COLOR_PAIR(YELLOW_BLACK_P));
                        }
                        break;
                case 4: /* white rook */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(YELLOW_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'R');
                            attroff(COLOR_PAIR(YELLOW_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(YELLOW_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'R');
                            attroff(COLOR_PAIR(YELLOW_BLACK_P));
                        }
                        break;
                case 5: /* white queen */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(YELLOW_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'Q');
                            attroff(COLOR_PAIR(YELLOW_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(YELLOW_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'Q');
                            attroff(COLOR_PAIR(YELLOW_BLACK_P));
                        }
                        break;
                case 6: /* white king */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(YELLOW_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'K');
                            attroff(COLOR_PAIR(YELLOW_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(YELLOW_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'K');
                            attroff(COLOR_PAIR(YELLOW_BLACK_P));
                        }
                        break;
                case 11: /* black pawn */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(BLUE_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'P');
                            attroff(COLOR_PAIR(BLUE_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(BLUE_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'P');
                            attroff(COLOR_PAIR(BLUE_BLACK_P));
                        }
                        break;
                case 12: /* black knight */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(BLUE_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'N');
                            attroff(COLOR_PAIR(BLUE_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(BLUE_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'N');
                            attroff(COLOR_PAIR(BLUE_BLACK_P));
                        }
                        break;
                case 13: /* black bishop */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(BLUE_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'B');
                            attroff(COLOR_PAIR(BLUE_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(BLUE_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'B');
                            attroff(COLOR_PAIR(BLUE_BLACK_P));
                        }
                        break;
                case 14: /* black rook */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(BLUE_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'R');
                            attroff(COLOR_PAIR(BLUE_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(BLUE_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'R');
                            attroff(COLOR_PAIR(BLUE_BLACK_P));
                        }
                        break;
                case 15: /* black queen */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(BLUE_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'Q');
                            attroff(COLOR_PAIR(BLUE_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(BLUE_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'Q');
                            attroff(COLOR_PAIR(BLUE_BLACK_P));
                        }
                        break;
                case 16: /* black king */
                        if(((i+j) % 2) == 0){
                            attron(COLOR_PAIR(BLUE_WHITE_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'K');
                            attroff(COLOR_PAIR(BLUE_WHITE_P));
                        }else{
                            attron(COLOR_PAIR(BLUE_BLACK_P));
                            mvaddch(OFFSET_Y+1+i*3, OFFSET_X+2+j*6, 'K');
                            attroff(COLOR_PAIR(BLUE_BLACK_P));
                        }
                        break;
                default:break;
            }
        }
    }

    mvaddstr(OFFSET_Y, OFFSET_X+54, "1 == enpassant");
    mvaddstr(OFFSET_Y+1, OFFSET_X+54, "2 == long castle w");
    mvaddstr(OFFSET_Y+2, OFFSET_X+54, "3 == long castle b");
    mvaddstr(OFFSET_Y+3, OFFSET_X+54, "4 == short castle w");
    mvaddstr(OFFSET_Y+4, OFFSET_X+54, "5 == short castle b");
    mvaddstr(OFFSET_Y+5, OFFSET_X+54, "6 == promote to Knight");
    mvaddstr(OFFSET_Y+6, OFFSET_X+54, "7 == promote to Bishop");
    mvaddstr(OFFSET_Y+7, OFFSET_X+54, "8 == promote to Rook");
    mvaddstr(OFFSET_Y+8, OFFSET_X+54, "9 == promote to Queen");

    attron(COLOR_PAIR(RED_P));
    for(int i = 0; i<8; i++){
        /* 49 for ascii offset */
        mvaddch(OFFSET_Y+1+i*3, OFFSET_X+49, (56-i));
    }
    mvaddstr(OFFSET_Y+25, OFFSET_X+2, "a     b     c     d     e     f     g     h");
    attroff(COLOR_PAIR(RED_P));


    refresh();
}

void create_board_small(void)
{
    /* vertical outside */
    for(int i = 0; i < 8; i++){
        /* horizontal outside */
        for(int j = 0; j < 8; j++){
            /* vertiakl inside */
            if((i % 2) == 0){
                if(j % 2 == 0){
                    /* white squares */
                    attron(COLOR_PAIR(WHITE_P));
                    mvaddstr(OFFSET_Y+i, OFFSET_X+2*j, "  ");
                    attroff(COLOR_PAIR(WHITE_P));
                }else{
                    /* black square */
                    attron(COLOR_PAIR(BLACK_P));
                    mvaddstr(OFFSET_Y+i, OFFSET_X+2*j, "  ");
                    attroff(COLOR_PAIR(BLACK_P));
                }
            }else{
                if(j % 2 == 0){
                    /* black square */
                    attron(COLOR_PAIR(BLACK_P));
                    mvaddstr(OFFSET_Y+i, OFFSET_X+2*j, "  ");
                    attroff(COLOR_PAIR(BLACK_P));
                }else{
                    /* white squares */
                    attron(COLOR_PAIR(WHITE_P));
                    mvaddstr(OFFSET_Y+i, OFFSET_X+2*j, "  ");
                    attroff(COLOR_PAIR(WHITE_P));
                }
            }
        }
    }
}

void create_board_large(void)
{
    /* vertical outside */
    for(int i = 0; i < 8; i++){
        /* horizontal outside */
        for(int j = 0; j < 8; j++){
            /* vertiakl inside */
            if((i % 2) == 0){
                if(j % 2 == 0){
                    /* white squares */
                    for(int k = 0; k < 3; k++){
                        attron(COLOR_PAIR(WHITE_P));
                        mvaddstr(OFFSET_Y+k+3*i, OFFSET_X+6*j, "      ");
                        attroff(COLOR_PAIR(WHITE_P));
                    }
                }else{
                    /* black square */
                    for(int k = 0; k < 3; k++){
                        attron(COLOR_PAIR(BLACK_P));
                        mvaddstr(OFFSET_Y+k+3*i, OFFSET_X+6*j, "      ");
                        attroff(COLOR_PAIR(BLACK_P));
                    }
                }
            }else{
                if(j % 2 == 0){
                    /* black square */
                    for(int k = 0; k < 3; k++){
                        attron(COLOR_PAIR(BLACK_P));
                        mvaddstr(OFFSET_Y+k+3*i, OFFSET_X+6*j, "      ");
                        attroff(COLOR_PAIR(BLACK_P));
                    }
                }else{
                    /* white squares */
                    for(int k = 0; k < 3; k++){
                        attron(COLOR_PAIR(WHITE_P));
                        mvaddstr(OFFSET_Y+k+3*i, OFFSET_X+6*j, "      ");
                        attroff(COLOR_PAIR(WHITE_P));
                    }
                }
            }
        }
    }
}

int init_curses(void)
{
    /* Init Screen */
    initscr();

    /* enable key-strokes*/
    keypad(stdscr, TRUE);

    /* disable print or pressed key*/
    noecho();

    atexit(quit);
    curs_set(0);

    /* Pieces are represented in a color */
    /* there for color in the terminal is needed*/
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        printf("Color support is necessary for the game\n");
        exit(EXIT_FAILURE);
    }
    /* Init colors in curses */
    start_color();

    /* Black pieces are blue and white pieces are yellow */
    init_pair(BLUE_BLACK_P, COLOR_BLUE, COLOR_BLACK);
    init_pair(BLUE_WHITE_P, COLOR_BLUE, COLOR_WHITE);
    init_pair(YELLOW_BLACK_P, COLOR_YELLOW, COLOR_BLACK);
    init_pair(YELLOW_WHITE_P, COLOR_YELLOW, COLOR_WHITE);
    init_pair(WHITE_P, COLOR_WHITE, COLOR_WHITE);
    init_pair(BLACK_P, COLOR_BLACK, COLOR_BLACK);
    init_pair(RED_P, COLOR_RED, COLOR_BLACK);


    clear();

    return EXIT_SUCCESS;
}

bool game_start(void)
{
    /* integer because it is represented as integer*/
    int input_char = 0; 
    bool player_color = 0;

    /* Start Screen */
    mvaddstr(3, 12, "*** Chess Engine - Welcome ***");
    mvaddstr(5, 6, "Do you think you can beat the engine ?");
    refresh();
    sleep(3);
    clear();

    /* Choose color  */
    mvaddstr(6, 6, "Choose your Color ! (White: \"w\" Black \"b\")");
    mvaddstr(7, 6, "To quit the Game press \"q\" or any other key");

    /* Ask for color */
    while((input_char = getch ()) != 'q'){
        if(input_char == 'w'){
                clear();
                player_color = false;
                mvaddstr(6, 6, "You did choose White !");
                refresh();
                break;
        }else if(input_char == 'b'){
                clear();
                player_color = true;
                mvaddstr(6, 6, "You did choose Black !");
                refresh();
                break;
        }else{
            clear();
            mvaddstr(6, 6, "Please choose either white(\"w\") or black(\"b\")");
            refresh();
        }
    }
    clear();
    create_board_large();
    
    refresh();

    return player_color;
}

void print_game_exit(int game_state, bool w_b)
{
    switch(game_state){
        case 2:
                mvaddstr(33, 6, "White won by checkmate !");
                break;
        case 3:
                mvaddstr(33, 6, "Black won by checkmate !");
                break;
        case 4:
                mvaddstr(33, 6, "Stalemate, it is a draw !");
                break;
        case 5:
                mvaddstr(33, 6, "Insufficient material, it is a draw !");
                break;
        case 6:
                mvaddstr(33, 6, "50 move rule, it is a draw !");
                break;
        default: break;
    }
    if(!w_b && game_state == 2)
        mvaddstr(34, 6, "Congratulations !");

    if(w_b && game_state == 2)
        mvaddstr(34, 6, "Looser !");

    if(!w_b && game_state == 3)
        mvaddstr(34, 6, "Looser !");

    if (w_b && game_state == 3)
        mvaddstr(34, 6, "Congratulations !");
            
    refresh();
    sleep(1);
    mvaddstr(36, 6, "press any key to quit the game !");
    refresh();
    getch();
}
