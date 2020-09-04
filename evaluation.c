#include <stdio.h>

int evaluation(int board[]) {
    int evaluation = 0;

    /* evaluation of captured pieces */
    for(int i = 20; i < 100; i++) {
        switch(board[i]) {
            case 1: evaluation+=100;break;
            case 11: evaluation-=100;break;
            case 2: evaluation+=280;break;
            case 3: evaluation+=300;break;
            case 4: evaluation+=500;break;
            case 5: evaluation+=900;break;
            case 6: evaluation+=100000;break;
            case 12: evaluation-=280;break;
            case 13: evaluation-=300;break;
            case 14: evaluation-=500;break;
            case 15: evaluation-=900;break;
            case 16: evaluation-=100000;break;
            default: break;
        }
    }
    printf("evaluation value: %d\n", evaluation);
    return evaluation;
}
