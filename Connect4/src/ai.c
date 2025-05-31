#include "connect4.h"
#include <stdlib.h>
#include <time.h>

int aiMove(GameBoard *board) {
    srand(time(NULL));
    int col;
    do {
        col = rand() % board->cols;
    } while (!isValidMove(board, col));
    return col;
}
