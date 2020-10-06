#include "board.h"
#include <limits.h>
/*int board[120] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
       -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
       -1,00,00,00,00,00,00,00,16,-1,
       -1,00,00,00,00,05,00,00,00,-1,
       -1,00,00,00,00,00,00,00,06,-1,
       -1,00,00,00,00,00,00,00,00,-1,
       -1,00,00,00,00,00,00,00,00,-1,
       -1,00,00,00,00,00,00,00,00,-1,
       -1,00,00,00,00,00,00,00,00,-1,
       -1,00,00,00,00,00,00,00,00,-1,
       -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
       -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};
*/
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

int long_castle_white = 1;
int long_castle_black = 1;
int short_castle_white = 1;
int short_castle_black = 1;
int move_50_rule = 0;



int legalmovecheckplayer()
{
    /*
	movegen(1); //depth=1
	if ((best_move.eval >50000) OR (best_move.eval <-50000))
	{
		return -1; 
	}
	return 1;
	//einzige möglichkeit ist es irgendwie den zug vom movegen finden zu lassen, sonst müssten wir an der stelle hier nochmal alle logik vom movegen hier einfügen.
*/
    return 1;
}

/* checkmatei */
int mate(bool color_to_move)
{
    /* 1 == no checkmate*/
    int checkmate = 1;
    int eval = move_gen(board, 2, INT_MIN, INT_MAX, color_to_move, false, false, false, false, false, NULL);

    if(eval > 50000){
        /* white checkmated black */
        checkmate = 2;
    }
    if(eval < -50000){
        /* black checkmated white */
        checkmate = 3;
    }
    /* stalemate */
    if(checkmate != 1){
        eval = move_gen(board, 1, INT_MIN, INT_MAX, !color_to_move, false, false, false, false, false, NULL);
        if(eval < 50000 && eval > -50000)
            checkmate = 4;
            
    }

    return checkmate;
}

/* insuficcient_material */
int insufficient_material()
{
    int material = 0, game_state = 5;

    /* Iterate over the board and count the pieces*/
    for(int i = 20; i < 100; i++){
        if(board[i]==1 || board[i]==4 ||  board[i]==5 || board[i]==11 || board[i]==14 || board[i]==15){
            game_state = 1;
            break;
        }

        /* Bishop and Knight have to be handled seperate */
        if(board[i]==2 || board[i]==3 || board[i]==12 || board[i]==13){
            material += 1;
            if(material >= 2){
                game_state = 1;
                break;
            }
        }
    }
    return game_state;
}

void make_move(int start_pos, int end_pos, int special_move, int *move_rule, bool *color_to_move, int *game_state)
{
    /* If piece is captured or a pawn is moved */
    if(board[end_pos] > 0 || board[start_pos] == 1 || board[start_pos] == 11)
        *move_rule = 0;
    /* 50 move ?*/

    switch(special_move) {
        case 0: /* normal move */
                board[end_pos] = board[start_pos];
                board[start_pos]=0;
                break;
        case 1: /* En passant*/
                board[end_pos]=board[start_pos];
                board[start_pos]=0;
                /* calculation of the pawn to capture */
                board[10*(start_pos/10)+end_pos%10]=0; 
                break;
        case 2: /* long castle white */
                board[91]=0;
                board[93]=6;
                board[94]=4;
                board[95]=0; 
                break;
        case 3: /* long castle black */
                board[21]=0;
                board[23]=16;
                board[24]=14;
                board[25]=0; 
                break;
        case 4: /* short castle white */
                board[95]=0;
                board[96]=4;
                board[97]=6;
                board[98]=0; 
                break;
        case 5: /* short castle black */
                board[25]=0;
                board[26]=14;
                board[27]=16;
                board[28]=0; 
                break;
        case 6: /* Promotion to Knight */
                board[end_pos] = 12;
                board[start_pos] = 0;
                if(end_pos<30)
                board[end_pos]=2;
                break;
        case 7: /* Promotion to Bishop */
                board[end_pos]=13;
                board[start_pos]=0;
                if(end_pos<30)
                board[end_pos]=3; 
                break;
        case 8: /* Promotion to Rook */
                board[end_pos]=14;
                board[start_pos]=0;
                if(end_pos<30)
                board[end_pos]=4; 
                break;
        case 9: /* Promotion to Queen */
                board[end_pos]=15;
                board[start_pos]=0;
                if(end_pos<30)
                board[end_pos]=5; 
                break;

        default:break;
    }

    /* toggle the color */
    if(*color_to_move == true)
        *color_to_move = false;
    else
        *color_to_move = true;

    /* checkmakte stalemante */
    *game_state = mate(*color_to_move);

    if (*game_state == 1) {
        /* insufficient material ?*/
        *game_state =  insufficient_material();

        /* check 50 move rule */
        *move_rule += 1;
        if(*move_rule>100){
            *game_state = 6;
        }
    }
}
