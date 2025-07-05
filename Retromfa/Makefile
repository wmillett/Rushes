CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude

SRCS = src/main.c src/retromfa.c
OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = retromfa
OBJDIR = obj

#TODO: remove this when the library is ready for use


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

run: $(TARGET)
	./$(TARGET)


.PHONY: all clean fclean re run bonus check-sdl2
