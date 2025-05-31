#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Function to validate and parse column input
int getValidColumn(int maxCol) {
    char input[100];
    char *endptr;
    long col;
    
    while (1) {
        printf("Enter column (0-%d): ", maxCol - 1);
        
        // Read entire line
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        
        // Remove newline character
        input[strcspn(input, "\n")] = 0;
        
        // Check for empty input
        if (strlen(input) == 0) {
            printf("Error: Empty input. Please enter a number.\n");
            continue;
        }
        
        // Check if input contains only digits (and optional leading/trailing spaces)
        char *trimmed = input;
        while (isspace(*trimmed)) trimmed++; // Skip leading spaces
        
        if (*trimmed == '\0') {
            printf("Error: Empty input. Please enter a number.\n");
            continue;
        }
        
        // Convert string to integer
        col = strtol(trimmed, &endptr, 10);
        
        // Check if conversion was successful (no remaining characters)
        while (isspace(*endptr)) endptr++; // Skip trailing spaces
        if (*endptr != '\0') {
            printf("Error: Invalid input '%s'. Please enter a number.\n", input);
            continue;
        }
        
        // Check if the number is within valid range
        if (col < 0 || col >= maxCol) {
            printf("Error: Column %ld is out of range. Please enter a number between 0 and %d.\n", col, maxCol - 1);
            continue;
        }
        
        return (int)col;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <rows> <cols>\n", argv[0]);
        printf("Example: %s 6 7\n", argv[0]);
        return 1;
    }

    // Validate that arguments are valid positive integers
    char *endptr1, *endptr2;
    long rows_long = strtol(argv[1], &endptr1, 10);
    long cols_long = strtol(argv[2], &endptr2, 10);
    
    if (*endptr1 != '\0' || *endptr2 != '\0') {
        printf("Error: Invalid input. Please provide valid integers for rows and columns.\n");
        printf("Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }
    
    if (rows_long <= 0 || cols_long <= 0) {
        printf("Error: Rows and columns must be positive integers.\n");
        printf("Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }
    
    int rows = (int)rows_long;
    int cols = (int)cols_long;

    // Validate minimum size
    if (rows < 6 || cols < 7) {
        printf("Error: Minimum size is 6 rows and 7 columns.\n");
        printf("Standard Connect 4 is played on a 6×7 grid.\n");
        return 1;
    }
    
    // Validate maximum size
    if (rows > 70 || cols > 71) {
        printf("Error: Maximum size is 70 rows and 71 columns.\n");
        printf("Requested size: %d×%d\n", rows, cols);
        printf("Please use a smaller grid size.\n");
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
    
    // Indicate who starts first
    if (currentPlayer == PLAYER) {
        printf("🎲 PLAYER 1 (\033[31m●\033[0m) starts first!\n");
    } else {
        printf("🎲 AI (\033[33m●\033[0m) starts first!\n");
    }
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
                col = getValidColumn(cols);
                if (!isValidMove(&board, col)) {
                    printf("Error: Column %d is full. Please choose another column.\n", col);
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
