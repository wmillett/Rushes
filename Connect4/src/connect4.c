#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>

void initBoard(GameBoard *board, int rows, int cols) {
    board->rows = rows;
    board->cols = cols;
    board->grid = (int **)ft_calloc(rows, PTR_SIZE);
    for (int i = 0; i < rows; i++) {
        board->grid[i] = (int *)ft_calloc(cols, INT_SIZE);
    }
}

void freeBoard(GameBoard *board) {
    for (int i = 0; i < board->rows; i++) {
        free(board->grid[i]);
    }
    free(board->grid);
}

void displayBoard(GameBoard *board) {
    // Print column numbers header
    ft_printf("  ");
    for (int j = 0; j < board->cols; j++) {
        ft_printf("%d ", j);
    }
    ft_printf("\n");
    
    // Print top border
    ft_printf("  ");
    for (int j = 0; j < board->cols; j++) {
        ft_printf("--");
    }
    ft_printf("-\n");
    
    // Print board with colors
    for (int i = 0; i < board->rows; i++) {
        ft_printf("| ");
        for (int j = 0; j < board->cols; j++) {
            if (board->grid[i][j] == PLAYER) {
                // Red for Player 1
                ft_printf("\033[31m●\033[0m ");
            } else if (board->grid[i][j] == AI) {
                // Yellow for AI/Player 2
                ft_printf("\033[33m●\033[0m ");
            } else {
                // Empty space
                ft_printf("· ");
            }
        }
        ft_printf("|\n");
    }
    
    // Print bottom border
    ft_printf("  ");
    for (int j = 0; j < board->cols; j++) {
        ft_printf("--");
    }
    ft_printf("-\n");
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
    // Check horizontal wins
    for (int row = 0; row < board->rows; row++) {
        for (int col = 0; col < board->cols - 3; col++) {
            if (board->grid[row][col] == player &&
                board->grid[row][col + 1] == player &&
                board->grid[row][col + 2] == player &&
                board->grid[row][col + 3] == player) {
                return 1;
            }
        }
    }
    
    // Check vertical wins
    for (int col = 0; col < board->cols; col++) {
        for (int row = 0; row < board->rows - 3; row++) {
            if (board->grid[row][col] == player &&
                board->grid[row + 1][col] == player &&
                board->grid[row + 2][col] == player &&
                board->grid[row + 3][col] == player) {
                return 1;
            }
        }
    }
    
    // Check positive diagonal wins (bottom-left to top-right)
    for (int row = 3; row < board->rows; row++) {
        for (int col = 0; col < board->cols - 3; col++) {
            if (board->grid[row][col] == player &&
                board->grid[row - 1][col + 1] == player &&
                board->grid[row - 2][col + 2] == player &&
                board->grid[row - 3][col + 3] == player) {
                return 1;
            }
        }
    }
    
    // Check negative diagonal wins (top-left to bottom-right)
    for (int row = 0; row < board->rows - 3; row++) {
        for (int col = 0; col < board->cols - 3; col++) {
            if (board->grid[row][col] == player &&
                board->grid[row + 1][col + 1] == player &&
                board->grid[row + 2][col + 2] == player &&
                board->grid[row + 3][col + 3] == player) {
                return 1;
            }
        }
    }
    
    return 0; // No win found
}

int isBoardFull(GameBoard *board) {
    for (int j = 0; j < board->cols; j++) {
        if (board->grid[0][j] == EMPTY) {
            return 0;
        }
    }
    return 1;
}
