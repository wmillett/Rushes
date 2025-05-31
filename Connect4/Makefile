CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude

SRCS = src/main.c src/connect4.c src/ai.c src/graphics.c
OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = connect4
OBJDIR = obj

# libftprintf library
LIBFTPRINTF_DIR = libftprintf
LIBFTPRINTF = $(LIBFTPRINTF_DIR)/libftprintf.a

# SDL2 libraries for bonus mode
SDL2_CFLAGS = $(shell sdl2-config --cflags 2>/dev/null)
SDL2_LIBS = $(shell sdl2-config --libs 2>/dev/null)

# Check if SDL2_ttf is available and add it if found
ifeq ($(shell pkg-config --exists sdl2_ttf && echo yes),yes)
    SDL2_LIBS += -lSDL2_ttf
    CFLAGS += -DSDL_TTF_AVAILABLE
endif

# Fallback if sdl2-config is not available
ifeq ($(SDL2_LIBS),)
    SDL2_LIBS = -lSDL2
endif

all: $(LIBFTPRINTF) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LIBFTPRINTF) $(SDL2_LIBS) -o $@

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(SDL2_CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBFTPRINTF):
	$(MAKE) -C $(LIBFTPRINTF_DIR)

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFTPRINTF_DIR) clean

fclean: clean
	rm -f $(TARGET)
	$(MAKE) -C $(LIBFTPRINTF_DIR) fclean

re: fclean all

run: $(TARGET)
	./$(TARGET) 6 7

# Run in bonus mode
bonus: $(TARGET)
	./$(TARGET) 6 7 bonus

# Check if SDL2 is installed
check-sdl2:
	@echo "Checking SDL2 installation..."
	@sdl2-config --version 2>/dev/null && echo "SDL2 found!" || echo "SDL2 not found. Install with: sudo apt-get install libsdl2-dev"
	@echo "Checking SDL2_ttf..."
	@pkg-config --exists sdl2_ttf 2>/dev/null && echo "SDL2_ttf found!" || echo "SDL2_ttf not found. Graphics will work without text."

.PHONY: all clean fclean re run bonus check-sdl2
