CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude

SRCS = src/main.c src/connect4.c src/ai.c
OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = connect4
OBJDIR = obj

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
