#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to validate and parse column input
int getValidColumn(int maxCol) {
    char input[100];
    char *endptr;
    long col;
    
    while (1) {
        ft_printf("Enter column (0-%d): ", maxCol - 1);
        
        // Read entire line
        if (fgets(input, sizeof(input), stdin) == NULL) {
            ft_printf("Error reading input. Please try again.\n");
            continue;
        }
        
        // Remove newline character
        input[ft_strcspn(input, "\n")] = 0;
        
        // Check for empty input
        if (ft_strlen(input) == 0) {
            ft_printf("Error: Empty input. Please enter a number.\n");
            continue;
        }
        
        // Check if input contains only digits (and optional leading/trailing spaces)
        char *trimmed = input;
        while (ft_isspace(*trimmed)) trimmed++; // Skip leading spaces
        
        if (*trimmed == '\0') {
            ft_printf("Error: Empty input. Please enter a number.\n");
            continue;
        }
        
        // Convert string to integer
        col = ft_strtol(trimmed, &endptr, 10);
        
        // Check if conversion was successful (no remaining characters)
        while (ft_isspace(*endptr)) endptr++; // Skip trailing spaces
        if (*endptr != '\0') {
            ft_printf("Error: Invalid input '%s'. Please enter a number.\n", input);
            continue;
        }
        
        // Check if the number is within valid range
        if (col < 0 || col >= maxCol) {
            ft_printf("Error: Column %d is out of range. Please enter a number between 0 and %d.\n", (int)col, maxCol - 1);
            continue;
        }
        
        return (int)col;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        ft_printf("Usage: %s <rows> <cols>\n", argv[0]);
        ft_printf("Example: %s 6 7\n", argv[0]);
        return 1;
    }

    // Validate that arguments are valid positive integers
    char *endptr1, *endptr2;
    long rows_long = ft_strtol(argv[1], &endptr1, 10);
    long cols_long = ft_strtol(argv[2], &endptr2, 10);
    
    if (*endptr1 != '\0' || *endptr2 != '\0') {
        ft_printf("Error: Invalid input. Please provide valid integers for rows and columns.\n");
        ft_printf("Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }
    
    if (rows_long <= 0 || cols_long <= 0) {
        ft_printf("Error: Rows and columns must be positive integers.\n");
        ft_printf("Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }
    
    int rows = (int)rows_long;
    int cols = (int)cols_long;

    // Validate minimum size
    if (rows < 6 || cols < 7) {
        ft_printf("Error: Minimum size is 6 rows and 7 columns.\n");
        ft_printf("Standard Connect 4 is played on a 6×7 grid.\n");
        return 1;
    }
    
    // Validate maximum size
    if (rows > 70 || cols > 71) {
        ft_printf("Error: Maximum size is 70 rows and 71 columns.\n");
        ft_printf("Requested size: %d×%d\n", rows, cols);
        ft_printf("Please use a smaller grid size.\n");
        return 1;
    }

    GameBoard board;
    initBoard(&board, rows, cols);

    srand(time(NULL));
    int currentPlayer = rand() % 2 + 1; // Randomly choose who starts

    // Game introduction
    ft_printf("\n");
    ft_printf("=================================================\n");
    ft_printf("           🔴 CONNECT 4 GAME 🟡\n");
    ft_printf("=================================================\n");
    ft_printf("Player 1: \033[31m●\033[0m (Red)    |    AI: \033[33m●\033[0m (Yellow)\n");
    ft_printf("=================================================\n");
    
    // Indicate who starts first
    if (currentPlayer == PLAYER) {
        ft_printf("🎲 PLAYER 1 (\033[31m●\033[0m) starts first!\n");
    } else {
        ft_printf("🎲 AI (\033[33m●\033[0m) starts first!\n");
    }
    ft_printf("\n");

    // Display initial board
    displayBoard(&board);

    while (1) {
        ft_printf("\n");
        ft_printf("-------------------------------------------------\n");
        
        if (currentPlayer == PLAYER) {
            ft_printf("🔴 PLAYER 1's TURN\n");
            ft_printf("-------------------------------------------------\n");
            int col;
            do {
                col = getValidColumn(cols);
                if (!isValidMove(&board, col)) {
                    ft_printf("Error: Column %d is full. Please choose another column.\n", col);
                }
            } while (!isValidMove(&board, col));
            makeMove(&board, col, PLAYER);
        } else {
            ft_printf("🟡 AI's TURN\n");
            ft_printf("-------------------------------------------------\n");
            int col = aiMove(&board);
            ft_printf("AI plays column %d\n", col);
            makeMove(&board, col, AI);
        }

        ft_printf("\n");
        displayBoard(&board);

        if (checkWin(&board, currentPlayer)) {
            ft_printf("\n");
            ft_printf("=================================================\n");
            if (currentPlayer == PLAYER) {
                ft_printf("🎉 PLAYER 1 (\033[31m●\033[0m) WINS! 🎉\n");
            } else {
                ft_printf("🤖 AI (\033[33m●\033[0m) WINS! 🤖\n");
            }
            ft_printf("=================================================\n");
            break;
        }

        if (isBoardFull(&board)) {
            ft_printf("\n");
            ft_printf("=================================================\n");
            ft_printf("🤝 IT'S A DRAW! 🤝\n");
            ft_printf("=================================================\n");
            break;
        }

        currentPlayer = (currentPlayer == PLAYER) ? AI : PLAYER;
    }

    freeBoard(&board);
    return 0;
}
