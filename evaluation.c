#include <stdio.h>
#include "evaluation.h"

int evaluation(int board[], int material) {
    int eval=material;
    /* evaluation of captured pieces */
    
    if(board[54] >0){
        eval += 10;
        if(board[54] > 10)
            eval -= 20;
    }

    if(board[55] >0){
        eval += 10;
        if(board[55] > 10)
            eval -= 20;
    }

    if(board[64] >0){
        eval += 10;
        if(board[56] > 10)
            eval -= 20;
    }

    if(board[65] >0){
        eval += 10;
        if(board[57] > 10)
            eval -= 20;
    }

    return eval;
}
