#ifndef CONNECT4_H
 #define CONNECT4_H

 #define EMPTY 0
 #define PLAYER 1
 #define AI 2
 
 typedef struct {
     int rows;
     int cols;
     int **grid;
 } GameBoard;
 
 void initBoard(GameBoard *board, int rows, int cols);
 void freeBoard(GameBoard *board);
 void displayBoard(GameBoard *board);
 int isValidMove(GameBoard *board, int col);
 void makeMove(GameBoard *board, int col, int player);
 int checkWin(GameBoard *board, int player);
 int isBoardFull(GameBoard *board);
 int aiMove(GameBoard *board); // Add this line to declare the aiMove function

#endif
