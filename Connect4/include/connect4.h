#ifndef CONNECT4_H
#define CONNECT4_H

#include "../libftprintf/ft_printf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0
#define PLAYER 1
#define AI 2

#define INPUT_BUFFER_SIZE 100
#define INT_SIZE 4
#define PTR_SIZE 8

typedef struct {
    int **grid;
    int rows;
    int cols;
} GameBoard;

// Function prototypes
void initBoard(GameBoard *board, int rows, int cols);
void freeBoard(GameBoard *board);
void displayBoard(GameBoard *board);
int isValidMove(GameBoard *board, int col);
void makeMove(GameBoard *board, int col, int player);
int checkWin(GameBoard *board, int player);
int isBoardFull(GameBoard *board);
int aiMove(GameBoard *board);

// Custom input function using get_next_line
int ft_getline_input(char *buffer, int max_size);

#endif
