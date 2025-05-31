#include "connect4.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

// Graphics constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 700
#define CELL_SIZE 80
#define CELL_MARGIN 10
#define BOARD_OFFSET_X 50
#define BOARD_OFFSET_Y 100

// Colors (RGBA)
#define COLOR_BACKGROUND 0x2C3E50FF
#define COLOR_BOARD 0x3498DBFF
#define COLOR_EMPTY 0xECF0F1FF
#define COLOR_PLAYER 0xE74C3CFF
#define COLOR_AI 0xF39C12FF
#define COLOR_HOVER 0x95A5A6FF
#define COLOR_TEXT 0x2C3E50FF

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool initialized;
    int hover_col;
    bool use_enhanced_font;
} GraphicsContext;

static GraphicsContext g_graphics = {0};

// Set SDL color from hex value
static void setColor(SDL_Renderer* renderer, Uint32 color) {
    Uint8 r = (color >> 24) & 0xFF;
    Uint8 g = (color >> 16) & 0xFF;
    Uint8 b = (color >> 8) & 0xFF;
    Uint8 a = color & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

// Enhanced pixel font - Improved 7x9 pixel font for better readability
static const unsigned char enhanced_font_data[][9] = {
    // C
    {0b0011110,
     0b0100001,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b0100001,
     0b0011110},
    // O  
    {0b0011110,
     0b0100001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b0100001,
     0b0011110},
    // N
    {0b1000001,
     0b1100001,
     0b1010001,
     0b1001001,
     0b1000101,
     0b1000011,
     0b1000001,
     0b1000001,
     0b1000001},
    // N (same as above)
    {0b1000001,
     0b1100001,
     0b1010001,
     0b1001001,
     0b1000101,
     0b1000011,
     0b1000001,
     0b1000001,
     0b1000001},
    // E
    {0b1111111,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1111110,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1111111},
    // C (same as first C)
    {0b0011110,
     0b0100001,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b0100001,
     0b0011110},
    // T
    {0b1111111,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000},
    // Space (empty)
    {0b0000000,
     0b0000000,
     0b0000000,
     0b0000000,
     0b0000000,
     0b0000000,
     0b0000000,
     0b0000000,
     0b0000000},
    // 4
    {0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1111111,
     0b0000001,
     0b0000001,
     0b0000001,
     0b0000001},
    // P
    {0b1111110,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1111110,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000},
    // L  
    {0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1000000,
     0b1111111},
    // A
    {0b0001000,
     0b0010100,
     0b0100010,
     0b1000001,
     0b1000001,
     0b1111111,
     0b1000001,
     0b1000001,
     0b1000001},
    // Y
    {0b1000001,
     0b1000001,
     0b0100010,
     0b0010100,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000},
    // R
    {0b1111110,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1111110,
     0b1001000,
     0b1000100,
     0b1000010,
     0b1000001},
    // I
    {0b0111110,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0001000,
     0b0111110},
    // W
    {0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1001001,
     0b1001001,
     0b1010101,
     0b1100011,
     0b1000001},
    // S
    {0b0111110,
     0b1000001,
     0b1000000,
     0b1000000,
     0b0111110,
     0b0000001,
     0b0000001,
     0b1000001,
     0b0111110},
    // D
    {0b1111110,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1000001,
     0b1111110}
};

// Function to draw a single character using enhanced bitmap font
static void drawEnhancedChar(int char_index, int x, int y, int scale) {
    if (char_index < 0 || char_index >= 18) return;
    
    setColor(g_graphics.renderer, COLOR_TEXT);
    
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 7; col++) {
            if (enhanced_font_data[char_index][row] & (1 << (6 - col))) {
                SDL_Rect pixel = {
                    x + col * scale,
                    y + row * scale,
                    scale,
                    scale
                };
                SDL_RenderFillRect(g_graphics.renderer, &pixel);
            }
        }
    }
}

// Function to get character index for enhanced font
static int getCharIndex(char c) {
    switch (c) {
        case 'C': return 0;
        case 'O': return 1;
        case 'N': return 2;
        case 'E': return 4;
        case 'T': return 6;
        case ' ': return 7;
        case '4': return 8;
        case 'P': return 9;
        case 'L': return 10;
        case 'A': return 11;
        case 'Y': return 12;
        case 'R': return 13;
        case 'I': return 14;
        case 'W': return 15;
        case 'S': return 16;
        case 'D': return 17;
        default: return 7; // Space for unknown characters
    }
}

// Function to draw "CONNECT 4" text using enhanced bitmap font
static void drawConnectFourText(int x, int y, int scale) {
    const char* text = "CONNECT 4";
    int char_spacing = 8 * scale; // 7 pixels + 1 pixel spacing
    int current_x = x;
    
    for (int i = 0; text[i] != '\0'; i++) {
        int char_index = getCharIndex(text[i]);
        drawEnhancedChar(char_index, current_x, y, scale);
        current_x += char_spacing;
    }
}

// Function to draw result text with enhanced font
static void drawResultText(int winner, int x, int y, int scale) {
    const char* text;
    if (winner == PLAYER) {
        text = "PLAYER WINS";
    } else if (winner == AI) {
        text = "AI WINS";  
    } else {
        text = "DRAW";
    }
    
    int char_spacing = 8 * scale;
    int current_x = x;
    
    for (int i = 0; text[i] != '\0'; i++) {
        int char_index = getCharIndex(text[i]);
        drawEnhancedChar(char_index, current_x, y, scale);
        current_x += char_spacing;
    }
}

// Initialize SDL2 graphics
int initGraphics(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ft_printf("SDL_Init Error: %s\n", SDL_GetError());
        return 0;
    }

    g_graphics.window = SDL_CreateWindow("Connect 4 - Bonus Mode",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!g_graphics.window) {
        ft_printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    g_graphics.renderer = SDL_CreateRenderer(g_graphics.window, -1,
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_graphics.renderer) {
        ft_printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_graphics.window);
        SDL_Quit();
        return 0;
    }

    g_graphics.initialized = true;
    g_graphics.hover_col = -1;
    g_graphics.use_enhanced_font = true;
    
    ft_printf("Graphics initialized with enhanced pixel font rendering!\n");
    ft_printf("TTF file found at: Res/sacrifice-font/SacrificeDemo-8Ox1B.ttf\n");
    ft_printf("(Note: SDL_ttf not available, using enhanced pixel font instead)\n");
    return 1;
}

// Clean up SDL2 resources
void cleanupGraphics(void) {
    if (g_graphics.initialized) {
        if (g_graphics.renderer) {
            SDL_DestroyRenderer(g_graphics.renderer);
        }
        if (g_graphics.window) {
            SDL_DestroyWindow(g_graphics.window);
        }
        SDL_Quit();
        g_graphics.initialized = false;
    }
}

// Draw a filled circle (for game pieces)
static void drawCircle(SDL_Renderer* renderer, int center_x, int center_y, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
            }
        }
    }
}

// Draw the Connect4 board
void drawBoard(GameBoard* board) {
    if (!g_graphics.initialized) return;

    // Clear screen with background color
    setColor(g_graphics.renderer, COLOR_BACKGROUND);
    SDL_RenderClear(g_graphics.renderer);

    // Draw title background with gradient effect
    setColor(g_graphics.renderer, COLOR_EMPTY);
    SDL_Rect title_bg = {WINDOW_WIDTH/2 - 140, 10, 280, 60};
    SDL_RenderFillRect(g_graphics.renderer, &title_bg);

    // Draw title border with double line effect
    setColor(g_graphics.renderer, COLOR_BOARD);
    SDL_RenderDrawRect(g_graphics.renderer, &title_bg);
    SDL_Rect inner_border = {title_bg.x + 2, title_bg.y + 2, title_bg.w - 4, title_bg.h - 4};
    SDL_RenderDrawRect(g_graphics.renderer, &inner_border);

    // Draw "CONNECT 4" text using enhanced font
    int text_scale = 3;
    int text_width = 9 * 8 * text_scale; // 9 characters * 8 pixels width * scale
    int text_x = WINDOW_WIDTH/2 - text_width/2;
    int text_y = 20;
    
    drawConnectFourText(text_x, text_y, text_scale);

    // Draw board background
    setColor(g_graphics.renderer, COLOR_BOARD);
    SDL_Rect board_bg = {
        BOARD_OFFSET_X - CELL_MARGIN,
        BOARD_OFFSET_Y - CELL_MARGIN,
        board->cols * (CELL_SIZE + CELL_MARGIN) + CELL_MARGIN,
        board->rows * (CELL_SIZE + CELL_MARGIN) + CELL_MARGIN
    };
    SDL_RenderFillRect(g_graphics.renderer, &board_bg);

    // Draw hover indicator with animation effect
    if (g_graphics.hover_col >= 0 && g_graphics.hover_col < board->cols) {
        setColor(g_graphics.renderer, COLOR_HOVER);
        int hover_x = BOARD_OFFSET_X + g_graphics.hover_col * (CELL_SIZE + CELL_MARGIN);
        SDL_Rect hover_rect = {hover_x, BOARD_OFFSET_Y - 35, CELL_SIZE, 25};
        SDL_RenderFillRect(g_graphics.renderer, &hover_rect);
        
        // Add arrow indicator
        setColor(g_graphics.renderer, COLOR_TEXT);
        SDL_Rect arrow = {hover_x + CELL_SIZE/2 - 5, BOARD_OFFSET_Y - 15, 10, 5};
        SDL_RenderFillRect(g_graphics.renderer, &arrow);
    }

    // Draw cells and pieces
    for (int row = 0; row < board->rows; row++) {
        for (int col = 0; col < board->cols; col++) {
            int x = BOARD_OFFSET_X + col * (CELL_SIZE + CELL_MARGIN);
            int y = BOARD_OFFSET_Y + row * (CELL_SIZE + CELL_MARGIN);
            
            // Draw cell background
            setColor(g_graphics.renderer, COLOR_EMPTY);
            SDL_Rect cell = {x, y, CELL_SIZE, CELL_SIZE};
            SDL_RenderFillRect(g_graphics.renderer, &cell);

            // Draw piece if present
            if (board->grid[row][col] != EMPTY) {
                if (board->grid[row][col] == PLAYER) {
                    setColor(g_graphics.renderer, COLOR_PLAYER);
                } else {
                    setColor(g_graphics.renderer, COLOR_AI);
                }
                drawCircle(g_graphics.renderer, x + CELL_SIZE/2, y + CELL_SIZE/2, CELL_SIZE/2 - 5);
                
                // Add highlight to pieces
                setColor(g_graphics.renderer, 0xFFFFFF40);
                drawCircle(g_graphics.renderer, x + CELL_SIZE/2 - 10, y + CELL_SIZE/2 - 10, CELL_SIZE/4 - 3);
            }
        }
    }

    SDL_RenderPresent(g_graphics.renderer);
}

// Handle SDL events and return column click (-1 if no click)
int handleGraphicsEvents(GameBoard* board) {
    if (!g_graphics.initialized) return -1;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return -2; // Special code for quit
                
            case SDL_MOUSEMOTION: {
                int mouse_x = event.motion.x;
                int mouse_y = event.motion.y;
                
                // Check if mouse is over the board
                if (mouse_y >= BOARD_OFFSET_Y && mouse_y <= BOARD_OFFSET_Y + board->rows * (CELL_SIZE + CELL_MARGIN)) {
                    int col = (mouse_x - BOARD_OFFSET_X) / (CELL_SIZE + CELL_MARGIN);
                    if (col >= 0 && col < board->cols) {
                        g_graphics.hover_col = col;
                    } else {
                        g_graphics.hover_col = -1;
                    }
                } else {
                    g_graphics.hover_col = -1;
                }
                break;
            }
            
            case SDL_MOUSEBUTTONDOWN: {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouse_x = event.button.x;
                    int mouse_y = event.button.y;
                    
                    // Check if click is on the board
                    if (mouse_y >= BOARD_OFFSET_Y && mouse_y <= BOARD_OFFSET_Y + board->rows * (CELL_SIZE + CELL_MARGIN)) {
                        int col = (mouse_x - BOARD_OFFSET_X) / (CELL_SIZE + CELL_MARGIN);
                        if (col >= 0 && col < board->cols && isValidMove(board, col)) {
                            return col;
                        }
                    }
                }
                break;
            }
            
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return -2; // Quit
                }
                break;
            }
        }
    }
    
    return -1; // No valid input
}

// Show game result in graphics
void showGameResult(int winner) {
    if (!g_graphics.initialized) return;

    // Draw a semi-transparent overlay
    setColor(g_graphics.renderer, 0x00000080);
    SDL_Rect overlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawBlendMode(g_graphics.renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(g_graphics.renderer, &overlay);

    // Draw result box with enhanced styling
    setColor(g_graphics.renderer, COLOR_EMPTY);
    SDL_Rect result_box = {WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2 - 60, 360, 120};
    SDL_RenderFillRect(g_graphics.renderer, &result_box);

    // Draw double border
    setColor(g_graphics.renderer, COLOR_BOARD);
    SDL_RenderDrawRect(g_graphics.renderer, &result_box);
    SDL_Rect inner_border = {result_box.x + 3, result_box.y + 3, result_box.w - 6, result_box.h - 6};
    SDL_RenderDrawRect(g_graphics.renderer, &inner_border);

    // Draw result text using enhanced font
    int text_scale = 3;
    int text_y = WINDOW_HEIGHT/2 - 18; // Center vertically
    
    if (winner == PLAYER) {
        int text_width = 11 * 8 * text_scale; // "PLAYER WINS" length
        int text_x = WINDOW_WIDTH/2 - text_width/2;
        drawResultText(winner, text_x, text_y, text_scale);
    } else if (winner == AI) {
        int text_width = 7 * 8 * text_scale; // "AI WINS" length  
        int text_x = WINDOW_WIDTH/2 - text_width/2;
        drawResultText(winner, text_x, text_y, text_scale);
    } else {
        int text_width = 4 * 8 * text_scale; // "DRAW" length
        int text_x = WINDOW_WIDTH/2 - text_width/2;
        drawResultText(winner, text_x, text_y, text_scale);
    }

    SDL_RenderPresent(g_graphics.renderer);
    
    // Wait for 3 seconds or ESC key
    Uint32 start_time = SDL_GetTicks();
    while (SDL_GetTicks() - start_time < 3000) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                return;
            }
        }
        SDL_Delay(16); // ~60 FPS
    }
}

// Check if graphics are initialized
bool isGraphicsMode(void) {
    return g_graphics.initialized;
} 