CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude

SRCS = src/main.c src/connect4.c src/ai.c
OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = connect4
OBJDIR = obj

# libftprintf library
LIBFTPRINTF_DIR = libftprintf
LIBFTPRINTF = $(LIBFTPRINTF_DIR)/libftprintf.a

all: $(LIBFTPRINTF) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LIBFTPRINTF) -o $@

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

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

.PHONY: all clean fclean re run
