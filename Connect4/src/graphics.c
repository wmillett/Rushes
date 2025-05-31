#include "connect4.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

// Try to include SDL_ttf, but make it optional
#ifdef SDL_TTF_AVAILABLE
#include <SDL2/SDL_ttf.h>
#endif

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
#ifdef SDL_TTF_AVAILABLE
    TTF_Font* font;
#endif
    bool initialized;
    bool has_text;
    int hover_col;
} GraphicsContext;

static GraphicsContext g_graphics = {0};

#ifdef SDL_TTF_AVAILABLE
// Helper function to render text (only if SDL_ttf is available)
static SDL_Texture* createTextTexture(const char* text, SDL_Color color) {
    if (!g_graphics.font) return NULL;
    
    SDL_Surface* text_surface = TTF_RenderText_Solid(g_graphics.font, text, color);
    if (!text_surface) {
        ft_printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return NULL;
    }
    
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(g_graphics.renderer, text_surface);
    SDL_FreeSurface(text_surface);
    
    if (!text_texture) {
        ft_printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    }
    
    return text_texture;
}
#endif

// Initialize SDL2 graphics
int initGraphics(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ft_printf("SDL_Init Error: %s\n", SDL_GetError());
        return 0;
    }

#ifdef SDL_TTF_AVAILABLE
    // Initialize SDL_ttf if available
    if (TTF_Init() < 0) {
        ft_printf("TTF_Init Error: %s\n", TTF_GetError());
        ft_printf("Continuing without text rendering...\n");
        g_graphics.has_text = false;
    } else {
        g_graphics.has_text = true;
    }
#else
    g_graphics.has_text = false;
    ft_printf("SDL_ttf not available. Graphics will work without text.\n");
#endif

    g_graphics.window = SDL_CreateWindow("Connect 4 - Bonus Mode",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!g_graphics.window) {
        ft_printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
#ifdef SDL_TTF_AVAILABLE
        if (g_graphics.has_text) TTF_Quit();
#endif
        SDL_Quit();
        return 0;
    }

    g_graphics.renderer = SDL_CreateRenderer(g_graphics.window, -1,
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_graphics.renderer) {
        ft_printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_graphics.window);
#ifdef SDL_TTF_AVAILABLE
        if (g_graphics.has_text) TTF_Quit();
#endif
        SDL_Quit();
        return 0;
    }

#ifdef SDL_TTF_AVAILABLE
    // Try to load a system font if TTF is available
    if (g_graphics.has_text) {
        const char* font_paths[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf", 
            "/usr/share/fonts/TTF/arial.ttf",
            "/System/Library/Fonts/Arial.ttf",  // macOS
            "C:\\Windows\\Fonts\\arial.ttf",    // Windows
            NULL
        };

        g_graphics.font = NULL;
        for (int i = 0; font_paths[i] != NULL; i++) {
            g_graphics.font = TTF_OpenFont(font_paths[i], 28);
            if (g_graphics.font) {
                break;
            }
        }

        if (!g_graphics.font) {
            ft_printf("Warning: Could not load any system font. Graphics will work without text.\n");
            g_graphics.has_text = false;
        }
    }
#endif

    g_graphics.initialized = true;
    g_graphics.hover_col = -1;
    return 1;
}

// Clean up SDL2 resources
void cleanupGraphics(void) {
    if (g_graphics.initialized) {
#ifdef SDL_TTF_AVAILABLE
        if (g_graphics.font) {
            TTF_CloseFont(g_graphics.font);
            g_graphics.font = NULL;
        }
        if (g_graphics.has_text) {
            TTF_Quit();
        }
#endif
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

// Set SDL color from hex value
static void setColor(SDL_Renderer* renderer, Uint32 color) {
    Uint8 r = (color >> 24) & 0xFF;
    Uint8 g = (color >> 16) & 0xFF;
    Uint8 b = (color >> 8) & 0xFF;
    Uint8 a = color & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
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

    // Draw title background
    setColor(g_graphics.renderer, COLOR_EMPTY);
    SDL_Rect title_bg = {WINDOW_WIDTH/2 - 120, 15, 240, 50};
    SDL_RenderFillRect(g_graphics.renderer, &title_bg);

    // Draw title border
    setColor(g_graphics.renderer, COLOR_BOARD);
    SDL_RenderDrawRect(g_graphics.renderer, &title_bg);

#ifdef SDL_TTF_AVAILABLE
    // Draw title text if font is available
    if (g_graphics.has_text && g_graphics.font) {
        SDL_Color text_color = {(COLOR_TEXT >> 24) & 0xFF, (COLOR_TEXT >> 16) & 0xFF, 
                               (COLOR_TEXT >> 8) & 0xFF, COLOR_TEXT & 0xFF};
        SDL_Texture* title_texture = createTextTexture("CONNECT 4", text_color);
        
        if (title_texture) {
            int text_w, text_h;
            SDL_QueryTexture(title_texture, NULL, NULL, &text_w, &text_h);
            SDL_Rect text_rect = {
                WINDOW_WIDTH/2 - text_w/2,
                15 + (50 - text_h)/2,
                text_w,
                text_h
            };
            SDL_RenderCopy(g_graphics.renderer, title_texture, NULL, &text_rect);
            SDL_DestroyTexture(title_texture);
        }
    } else {
        // Draw simple pattern if no text available
        setColor(g_graphics.renderer, COLOR_BOARD);
        for (int i = 0; i < 4; i++) {
            SDL_Rect dot = {WINDOW_WIDTH/2 - 60 + i * 30, 35, 10, 10};
            SDL_RenderFillRect(g_graphics.renderer, &dot);
        }
    }
#else
    // Draw simple pattern if no text available
    setColor(g_graphics.renderer, COLOR_BOARD);
    for (int i = 0; i < 4; i++) {
        SDL_Rect dot = {WINDOW_WIDTH/2 - 60 + i * 30, 35, 10, 10};
        SDL_RenderFillRect(g_graphics.renderer, &dot);
    }
#endif

    // Draw board background
    setColor(g_graphics.renderer, COLOR_BOARD);
    SDL_Rect board_bg = {
        BOARD_OFFSET_X - CELL_MARGIN,
        BOARD_OFFSET_Y - CELL_MARGIN,
        board->cols * (CELL_SIZE + CELL_MARGIN) + CELL_MARGIN,
        board->rows * (CELL_SIZE + CELL_MARGIN) + CELL_MARGIN
    };
    SDL_RenderFillRect(g_graphics.renderer, &board_bg);

    // Draw hover indicator
    if (g_graphics.hover_col >= 0 && g_graphics.hover_col < board->cols) {
        setColor(g_graphics.renderer, COLOR_HOVER);
        int hover_x = BOARD_OFFSET_X + g_graphics.hover_col * (CELL_SIZE + CELL_MARGIN);
        SDL_Rect hover_rect = {hover_x, BOARD_OFFSET_Y - 30, CELL_SIZE, 20};
        SDL_RenderFillRect(g_graphics.renderer, &hover_rect);
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

    // Draw result box
    setColor(g_graphics.renderer, COLOR_EMPTY);
    SDL_Rect result_box = {WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 50, 300, 100};
    SDL_RenderFillRect(g_graphics.renderer, &result_box);

    // Draw border
    setColor(g_graphics.renderer, COLOR_BOARD);
    SDL_RenderDrawRect(g_graphics.renderer, &result_box);

#ifdef SDL_TTF_AVAILABLE
    // Draw result text if font is available
    if (g_graphics.has_text && g_graphics.font) {
        SDL_Color text_color = {(COLOR_TEXT >> 24) & 0xFF, (COLOR_TEXT >> 16) & 0xFF, 
                               (COLOR_TEXT >> 8) & 0xFF, COLOR_TEXT & 0xFF};
        
        const char* result_text;
        if (winner == PLAYER) {
            result_text = "PLAYER WINS!";
        } else if (winner == AI) {
            result_text = "AI WINS!";
        } else {
            result_text = "DRAW!";
        }
        
        SDL_Texture* result_texture = createTextTexture(result_text, text_color);
        if (result_texture) {
            int text_w, text_h;
            SDL_QueryTexture(result_texture, NULL, NULL, &text_w, &text_h);
            SDL_Rect text_rect = {
                WINDOW_WIDTH/2 - text_w/2,
                WINDOW_HEIGHT/2 - text_h/2,
                text_w,
                text_h
            };
            SDL_RenderCopy(g_graphics.renderer, result_texture, NULL, &text_rect);
            SDL_DestroyTexture(result_texture);
        }
    } else {
        // Draw simple visual indicators if no text
        setColor(g_graphics.renderer, winner == PLAYER ? COLOR_PLAYER : 
                winner == AI ? COLOR_AI : COLOR_BOARD);
        SDL_Rect indicator = {WINDOW_WIDTH/2 - 20, WINDOW_HEIGHT/2 - 20, 40, 40};
        SDL_RenderFillRect(g_graphics.renderer, &indicator);
    }
#else
    // Draw simple visual indicators if no text
    setColor(g_graphics.renderer, winner == PLAYER ? COLOR_PLAYER : 
            winner == AI ? COLOR_AI : COLOR_BOARD);
    SDL_Rect indicator = {WINDOW_WIDTH/2 - 20, WINDOW_HEIGHT/2 - 20, 40, 40};
    SDL_RenderFillRect(g_graphics.renderer, &indicator);
#endif

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