#include "connect4.h"
#include <stdlib.h>
#include <time.h>

#define MAX_DEPTH 6
#define INFINITE 1000000

// Function prototypes for helper functions
static int evaluateBoard(GameBoard *board, int player);
static int evaluateWindow(int window[4], int player);
static int minimax(GameBoard *board, int depth, int alpha, int beta, int maximizingPlayer, int aiPlayer);
static int isTerminalNode(GameBoard *board);
static void copyBoard(GameBoard *source, GameBoard *dest);
static int getNextOpenRow(GameBoard *board, int col);

// Get the opponent of the current player
static int getOpponent(int player) {
    return (player == PLAYER) ? AI : PLAYER;
}

// Check if current board state is a terminal node (game over)
static int isTerminalNode(GameBoard *board) {
    return checkWin(board, PLAYER) || checkWin(board, AI) || isBoardFull(board);
}

// Get the next available row for a column (simulates gravity)
static int getNextOpenRow(GameBoard *board, int col) {
    for (int r = board->rows - 1; r >= 0; r--) {
        if (board->grid[r][col] == EMPTY) {
            return r;
        }
    }
    return -1; // Column is full
}

// Copy board state (for minimax simulation)
static void copyBoard(GameBoard *source, GameBoard *dest) {
    dest->rows = source->rows;
    dest->cols = source->cols;
    for (int i = 0; i < source->rows; i++) {
        for (int j = 0; j < source->cols; j++) {
            dest->grid[i][j] = source->grid[i][j];
        }
    }
}

// Evaluate a window of 4 positions for scoring
static int evaluateWindow(int window[4], int player) {
    int score = 0;
    int opponent = getOpponent(player);
    int player_count = 0;
    int opponent_count = 0;
    int empty_count = 0;

    // Count pieces in the window
    for (int i = 0; i < 4; i++) {
        if (window[i] == player) {
            player_count++;
        } else if (window[i] == opponent) {
            opponent_count++;
        } else {
            empty_count++;
        }
    }

    // Score the window based on piece configuration
    if (player_count == 4) {
        score += 100;  // Winning position
    } else if (player_count == 3 && empty_count == 1) {
        score += 10;   // Very good position
    } else if (player_count == 2 && empty_count == 2) {
        score += 2;    // Good position
    }

    // Penalize opponent's strong positions
    if (opponent_count == 3 && empty_count == 1) {
        score -= 80;   // Block opponent's winning move
    }

    return score;
}

// Evaluate the entire board position for a player
static int evaluateBoard(GameBoard *board, int player) {
    int score = 0;
    int window[4];

    // Score center column preference (strategic advantage)
    int center_col = board->cols / 2;
    int center_count = 0;
    for (int r = 0; r < board->rows; r++) {
        if (board->grid[r][center_col] == player) {
            center_count++;
        }
    }
    score += center_count * 3;

    // Score horizontal positions
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols - 3; c++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board->grid[r][c + i];
            }
            score += evaluateWindow(window, player);
        }
    }

    // Score vertical positions
    for (int c = 0; c < board->cols; c++) {
        for (int r = 0; r < board->rows - 3; r++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board->grid[r + i][c];
            }
            score += evaluateWindow(window, player);
        }
    }

    // Score positive diagonal positions (bottom-left to top-right)
    for (int r = 0; r < board->rows - 3; r++) {
        for (int c = 0; c < board->cols - 3; c++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board->grid[r + 3 - i][c + i];
            }
            score += evaluateWindow(window, player);
        }
    }

    // Score negative diagonal positions (top-left to bottom-right)
    for (int r = 0; r < board->rows - 3; r++) {
        for (int c = 0; c < board->cols - 3; c++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board->grid[r + i][c + i];
            }
            score += evaluateWindow(window, player);
        }
    }

    return score;
}

// Minimax algorithm with alpha-beta pruning
static int minimax(GameBoard *board, int depth, int alpha, int beta, int maximizingPlayer, int aiPlayer) {
    int is_terminal = isTerminalNode(board);

    if (depth == 0 || is_terminal) {
        if (is_terminal) {
            if (checkWin(board, aiPlayer)) {
                return 100000000;  // AI wins - very high score
            } else if (checkWin(board, getOpponent(aiPlayer))) {
                return -100000000; // AI loses - very low score
            } else {
                return 0; // Draw
            }
        } else {
            // Return the board evaluation
            return evaluateBoard(board, aiPlayer);
        }
    }

    if (maximizingPlayer) {
        int maxEval = -INFINITE;
        for (int col = 0; col < board->cols; col++) {
            if (isValidMove(board, col)) {
                // Create temporary board for simulation
                GameBoard tempBoard;
                tempBoard.grid = (int **)ft_calloc(board->rows, PTR_SIZE);
                for (int i = 0; i < board->rows; i++) {
                    tempBoard.grid[i] = (int *)ft_calloc(board->cols, INT_SIZE);
                }
                copyBoard(board, &tempBoard);

                // Make the move
                int row = getNextOpenRow(&tempBoard, col);
                tempBoard.grid[row][col] = aiPlayer;

                // Recursive minimax call
                int eval = minimax(&tempBoard, depth - 1, alpha, beta, 0, aiPlayer);
                
                // Free temporary board
                for (int i = 0; i < tempBoard.rows; i++) {
                    free(tempBoard.grid[i]);
                }
                free(tempBoard.grid);

                maxEval = (eval > maxEval) ? eval : maxEval;
                alpha = (alpha > eval) ? alpha : eval;
                if (beta <= alpha) {
                    break; // Alpha-beta pruning
                }
            }
        }
        return maxEval;
    } else {
        int minEval = INFINITE;
        int opponent = getOpponent(aiPlayer);
        for (int col = 0; col < board->cols; col++) {
            if (isValidMove(board, col)) {
                // Create temporary board for simulation
                GameBoard tempBoard;
                tempBoard.grid = (int **)ft_calloc(board->rows, PTR_SIZE);
                for (int i = 0; i < board->rows; i++) {
                    tempBoard.grid[i] = (int *)ft_calloc(board->cols, INT_SIZE);
                }
                copyBoard(board, &tempBoard);

                // Make the move
                int row = getNextOpenRow(&tempBoard, col);
                tempBoard.grid[row][col] = opponent;

                // Recursive minimax call
                int eval = minimax(&tempBoard, depth - 1, alpha, beta, 1, aiPlayer);
                
                // Free temporary board
                for (int i = 0; i < tempBoard.rows; i++) {
                    free(tempBoard.grid[i]);
                }
                free(tempBoard.grid);

                minEval = (eval < minEval) ? eval : minEval;
                beta = (beta < eval) ? beta : eval;
                if (beta <= alpha) {
                    break; // Alpha-beta pruning
                }
            }
        }
        return minEval;
    }
}

// Main AI move function - finds the best move using minimax
int aiMove(GameBoard *board) {
    int bestCol = 0;
    int bestScore = -INFINITE;

    // First, check for immediate winning moves
    for (int col = 0; col < board->cols; col++) {
        if (isValidMove(board, col)) {
            // Create temporary board
            GameBoard tempBoard;
            tempBoard.grid = (int **)ft_calloc(board->rows, PTR_SIZE);
            for (int i = 0; i < board->rows; i++) {
                tempBoard.grid[i] = (int *)ft_calloc(board->cols, INT_SIZE);
            }
            copyBoard(board, &tempBoard);

            // Simulate the move
            int row = getNextOpenRow(&tempBoard, col);
            tempBoard.grid[row][col] = AI;

            // Check if this move wins
            if (checkWin(&tempBoard, AI)) {
                // Free temporary board
                for (int i = 0; i < tempBoard.rows; i++) {
                    free(tempBoard.grid[i]);
                }
                free(tempBoard.grid);
                return col; // Take the winning move immediately
            }
            
            // Free temporary board
            for (int i = 0; i < tempBoard.rows; i++) {
                free(tempBoard.grid[i]);
            }
            free(tempBoard.grid);
        }
    }

    // Second, check for blocking opponent's winning moves
    for (int col = 0; col < board->cols; col++) {
        if (isValidMove(board, col)) {
            // Create temporary board
            GameBoard tempBoard;
            tempBoard.grid = (int **)ft_calloc(board->rows, PTR_SIZE);
            for (int i = 0; i < board->rows; i++) {
                tempBoard.grid[i] = (int *)ft_calloc(board->cols, INT_SIZE);
            }
            copyBoard(board, &tempBoard);

            // Simulate opponent's move
            int row = getNextOpenRow(&tempBoard, col);
            tempBoard.grid[row][col] = PLAYER;

            // Check if opponent would win
            if (checkWin(&tempBoard, PLAYER)) {
                bestCol = col; // Block the opponent
                bestScore = 50000; // High priority for blocking
            }
            
            // Free temporary board
            for (int i = 0; i < tempBoard.rows; i++) {
                free(tempBoard.grid[i]);
            }
            free(tempBoard.grid);
        }
    }

    // If we found a blocking move and no winning move, use it
    if (bestScore == 50000) {
        return bestCol;
    }

    // Use minimax to find the best strategic move
    for (int col = 0; col < board->cols; col++) {
        if (isValidMove(board, col)) {
            // Create temporary board
            GameBoard tempBoard;
            tempBoard.grid = (int **)ft_calloc(board->rows, PTR_SIZE);
            for (int i = 0; i < board->rows; i++) {
                tempBoard.grid[i] = (int *)ft_calloc(board->cols, INT_SIZE);
            }
            copyBoard(board, &tempBoard);

            // Make the move
            int row = getNextOpenRow(&tempBoard, col);
            tempBoard.grid[row][col] = AI;

            // Get the minimax score for this move
            int score = minimax(&tempBoard, MAX_DEPTH - 1, -INFINITE, INFINITE, 0, AI);

            // Free temporary board
            for (int i = 0; i < tempBoard.rows; i++) {
                free(tempBoard.grid[i]);
            }
            free(tempBoard.grid);

            // Update best move if this is better
            if (score > bestScore) {
                bestScore = score;
                bestCol = col;
            }
        }
    }

    return bestCol;
}
