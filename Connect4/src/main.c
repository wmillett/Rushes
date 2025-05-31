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

    // Game introduction
    printf("\n");
    printf("=================================================\n");
    printf("           🔴 CONNECT 4 GAME 🟡\n");
    printf("=================================================\n");
    printf("Player 1: \033[31m●\033[0m (Red)    |    AI: \033[33m●\033[0m (Yellow)\n");
    printf("=================================================\n");
    printf("\n");

    // Display initial board
    displayBoard(&board);

    while (1) {
        printf("\n");
        printf("-------------------------------------------------\n");
        
        if (currentPlayer == PLAYER) {
            printf("🔴 PLAYER 1's TURN\n");
            printf("-------------------------------------------------\n");
            int col;
            do {
                printf("Enter column (0-%d): ", cols - 1);
                scanf("%d", &col);
                if (!isValidMove(&board, col)) {
                    printf("Invalid move! Try again.\n");
                }
            } while (!isValidMove(&board, col));
            makeMove(&board, col, PLAYER);
        } else {
            printf("🟡 AI's TURN\n");
            printf("-------------------------------------------------\n");
            int col = aiMove(&board);
            printf("AI plays column %d\n", col);
            makeMove(&board, col, AI);
        }

        printf("\n");
        displayBoard(&board);

        if (checkWin(&board, currentPlayer)) {
            printf("\n");
            printf("=================================================\n");
            if (currentPlayer == PLAYER) {
                printf("🎉 PLAYER 1 (\033[31m●\033[0m) WINS! 🎉\n");
            } else {
                printf("🤖 AI (\033[33m●\033[0m) WINS! 🤖\n");
            }
            printf("=================================================\n");
            break;
        }

        if (isBoardFull(&board)) {
            printf("\n");
            printf("=================================================\n");
            printf("🤝 IT'S A DRAW! 🤝\n");
            printf("=================================================\n");
            break;
        }

        currentPlayer = (currentPlayer == PLAYER) ? AI : PLAYER;
    }

    freeBoard(&board);
    return 0;
}
