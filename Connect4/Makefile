CC = cc
CFLAGS = -Wall
INCLUDES = -Iinclude

SRCS = src/main.c src/connect4.c src/ai.c
OBJS = $(SRCS:.c=.o)
TARGET = connect4

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
