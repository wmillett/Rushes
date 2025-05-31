#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Custom input function using get_next_line (replaces fgets + stdin)
int ft_getline_input(char *buffer, int max_size) {
    char *line;
    int len;
    int i;
    
    // Read a line from stdin (file descriptor 0) using get_next_line
    line = get_next_line(0);
    if (!line) {
        return -1; // Error or EOF
    }
    
    // Calculate length and copy to buffer
    len = ft_strlen(line);
    
    // Remove newline if present
    if (len > 0 && line[len - 1] == '\n') {
        len--;
    }
    
    // Ensure we don't exceed buffer size
    if (len >= max_size) {
        len = max_size - 1;
    }
    
    // Copy the line to buffer
    for (i = 0; i < len; i++) {
        buffer[i] = line[i];
    }
    buffer[len] = '\0';
    
    // Free the line allocated by get_next_line
    free(line);
    
    return len;
}

// Function to validate and parse column input
int getValidColumn(int maxCol) {
    char input[INPUT_BUFFER_SIZE];
    char *endptr;
    long col;
    
    while (1) {
        ft_printf("Enter column (0-%d): ", maxCol - 1);
        
        // Read entire line using custom function
        if (ft_getline_input(input, INPUT_BUFFER_SIZE) < 0) {
            ft_printf("Error reading input. Please try again.\n");
            continue;
        }
        
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
    bool graphics_mode = false;
    
    // Check for bonus argument
    if (argc == 4 && ft_strncmp(argv[3], "bonus", 5) == 0) {
        graphics_mode = true;
    } else if (argc != 3) {
        ft_printf("Usage: %s <rows> <cols> [bonus]\n", argv[0]);
        ft_printf("Example: %s 6 7\n", argv[0]);
        ft_printf("Example: %s 6 7 bonus\n", argv[0]);
        return 1;
    }

    // Validate that arguments are valid positive integers
    char *endptr1, *endptr2;
    long rows_long = ft_strtol(argv[1], &endptr1, 10);
    long cols_long = ft_strtol(argv[2], &endptr2, 10);
    
    if (*endptr1 != '\0' || *endptr2 != '\0') {
        ft_printf("Error: Invalid input. Please provide valid integers for rows and columns.\n");
        ft_printf("Usage: %s <rows> <cols> [bonus]\n", argv[0]);
        return 1;
    }
    
    if (rows_long <= 0 || cols_long <= 0) {
        ft_printf("Error: Rows and columns must be positive integers.\n");
        ft_printf("Usage: %s <rows> <cols> [bonus]\n", argv[0]);
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

    // Initialize graphics if bonus mode
    if (graphics_mode) {
        if (!initGraphics()) {
            ft_printf("Failed to initialize graphics. Falling back to terminal mode.\n");
            graphics_mode = false;
        } else {
            ft_printf("🎮 BONUS MODE: Graphical interface enabled!\n");
            ft_printf("💡 Click on columns to make your move, or use ESC to quit.\n");
        }
    }

    GameBoard board;
    initBoard(&board, rows, cols);

    srand(time(NULL));
    int currentPlayer = rand() % 2 + 1; // Randomly choose who starts

    // Game introduction
    ft_printf("\n");
    ft_printf("=================================================\n");
    if (graphics_mode) {
        ft_printf("       🔴 CONNECT 4 GAME - BONUS MODE 🟡\n");
    } else {
        ft_printf("           🔴 CONNECT 4 GAME 🟡\n");
    }
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
    if (graphics_mode) {
        drawBoard(&board);
    }

    while (1) {
        ft_printf("\n");
        ft_printf("-------------------------------------------------\n");
        
        if (currentPlayer == PLAYER) {
            ft_printf("🔴 PLAYER 1's TURN\n");
            ft_printf("-------------------------------------------------\n");
            
            int col = -1;
            
            if (graphics_mode) {
                ft_printf("Click on a column in the graphics window to make your move...\n");
                
                // Wait for player input from graphics
                while (col == -1) {
                    col = handleGraphicsEvents(&board);
                    
                    if (col == -2) { // Quit requested
                        ft_printf("Game ended by user.\n");
                        goto cleanup;
                    }
                    
                    if (col >= 0 && !isValidMove(&board, col)) {
                        ft_printf("Column %d is full. Please choose another column.\n", col);
                        col = -1;
                    }
                    
                    // Small delay to prevent 100% CPU usage
                    if (graphics_mode) {
                        drawBoard(&board); // Refresh graphics for hover effects
                    }
                }
            } else {
                // Terminal input mode
                do {
                    col = getValidColumn(cols);
                    if (!isValidMove(&board, col)) {
                        ft_printf("Error: Column %d is full. Please choose another column.\n", col);
                    }
                } while (!isValidMove(&board, col));
            }
            
            ft_printf("Player 1 plays column %d\n", col);
            makeMove(&board, col, PLAYER);
        } else {
            ft_printf("🟡 AI's TURN\n");
            ft_printf("-------------------------------------------------\n");
            int col = aiMove(&board);
            ft_printf("AI plays column %d\n", col);
            makeMove(&board, col, AI);
            
            // Small delay so player can see AI move
            if (graphics_mode) {
                // Process any pending graphics events during AI turn
                int dummy_col;
                do {
                    dummy_col = handleGraphicsEvents(&board);
                    if (dummy_col == -2) { // Quit requested
                        ft_printf("Game ended by user.\n");
                        goto cleanup;
                    }
                } while (dummy_col >= 0); // Ignore valid moves during AI turn
            }
        }

        ft_printf("\n");
        displayBoard(&board);
        if (graphics_mode) {
            drawBoard(&board);
        }

        if (checkWin(&board, currentPlayer)) {
            ft_printf("\n");
            ft_printf("=================================================\n");
            if (currentPlayer == PLAYER) {
                ft_printf("🎉 PLAYER 1 (\033[31m●\033[0m) WINS! 🎉\n");
            } else {
                ft_printf("🤖 AI (\033[33m●\033[0m) WINS! 🤖\n");
            }
            ft_printf("=================================================\n");
            
            if (graphics_mode) {
                showGameResult(currentPlayer);
            }
            break;
        }

        if (isBoardFull(&board)) {
            ft_printf("\n");
            ft_printf("=================================================\n");
            ft_printf("🤝 IT'S A DRAW! 🤝\n");
            ft_printf("=================================================\n");
            
            if (graphics_mode) {
                showGameResult(0); // 0 for draw
            }
            break;
        }

        currentPlayer = (currentPlayer == PLAYER) ? AI : PLAYER;
    }

cleanup:
    freeBoard(&board);
    if (graphics_mode) {
        cleanupGraphics();
    }
    cleanup_get_next_line();  // Clean up get_next_line static memory
    return 0;
}
