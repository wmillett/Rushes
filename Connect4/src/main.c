#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    if (rows < 6 || cols < 7) {
        printf("Minimum size is 6 lines and 7 columns.\n");
        return 1;
    }

    GameBoard board;
    initBoard(&board, rows, cols);

    srand(time(NULL));
    int currentPlayer = rand() % 2 + 1; // Randomly choose who starts

    while (1) {
        displayBoard(&board);

        if (currentPlayer == PLAYER) {
            int col;
            do {
                printf("Enter column (0-%d): ", cols - 1);
                scanf("%d", &col);
            } while (!isValidMove(&board, col));
            makeMove(&board, col, PLAYER);
        } else {
            int col = aiMove(&board);
            printf("AI plays column %d\n", col);
            makeMove(&board, col, AI);
        }

        if (checkWin(&board, currentPlayer)) {
            displayBoard(&board);
            printf("Player %d wins!\n", currentPlayer);
            break;
        }

        if (isBoardFull(&board)) {
            displayBoard(&board);
            printf("It's a draw!\n");
            break;
        }

        currentPlayer = (currentPlayer == PLAYER) ? AI : PLAYER;
    }

    freeBoard(&board);
    return 0;
}
