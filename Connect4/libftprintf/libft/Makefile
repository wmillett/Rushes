# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/11 20:14:03 by wmillett          #+#    #+#              #
#    Updated: 2023/12/18 22:26:39 by wmillett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC =  ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
ft_atoi.c ft_strdup.c ft_calloc.c ft_substr.c ft_strjoin.c \
ft_strtrim.c ft_itoa.c ft_split.c ft_putchar_fd.c ft_putstr_fd.c \
ft_putendl_fd.c ft_putnbr_fd.c ft_strmapi.c ft_striteri.c

SRC_BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

OBJ = $(SRC:.c=.o) 
OBJ_BONUS = $(SRC_BONUS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I.
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	@ar -rcs $(NAME) $^

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $^
	
bonus:
	@$(MAKE) "SRC=$(SRC) $(SRC_BONUS)"
	
clean:
	@$(RM) $(OBJ) $(OBJ_BONUS)
	
fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
