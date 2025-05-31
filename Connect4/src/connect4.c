#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>

void initBoard(GameBoard *board, int rows, int cols) {
    board->rows = rows;
    board->cols = cols;
    board->grid = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        board->grid[i] = (int *)calloc(cols, sizeof(int));
    }
}

void freeBoard(GameBoard *board) {
    for (int i = 0; i < board->rows; i++) {
        free(board->grid[i]);
    }
    free(board->grid);
}

void displayBoard(GameBoard *board) {
    for (int i = 0; i < board->rows; i++) {
        for (int j = 0; j < board->cols; j++) {
            printf("%d ", board->grid[i][j]);
        }
        printf("\n");
    }
}

int isValidMove(GameBoard *board, int col) {
    return col >= 0 && col < board->cols && board->grid[0][col] == EMPTY;
}

void makeMove(GameBoard *board, int col, int player) {
    for (int i = board->rows - 1; i >= 0; i--) {
        if (board->grid[i][col] == EMPTY) {
            board->grid[i][col] = player;
            break;
        }
    }
}

int checkWin(GameBoard *board, int player) {
    // Implement win checking logic
    return 0; // Placeholder
}

int isBoardFull(GameBoard *board) {
    for (int j = 0; j < board->cols; j++) {
        if (board->grid[0][j] == EMPTY) {
            return 0;
        }
    }
    return 1;
}
