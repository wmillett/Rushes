CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = retromfa
OBJDIR = obj

MFAFILES = $(wildcard assets/MFA/*.mfa)
TESTFILES = $(wildcard assets/testerror/*)

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

testerror: $(TARGET)
	@echo "Running tests..."
	./$(TARGET) $(TESTFILES)


run: $(TARGET)
	./$(TARGET) $(MFAFILES)


.PHONY: all clean fclean re run testerror
