# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 15:04:35 by wmillett          #+#    #+#              #
#    Updated: 2023/03/15 16:25:36 by wmillett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = libftprintf.a
LIBFT = libft.a
LPATH = libft/
GPATH = getnextline/
SRC = ft_printf.c\
		ft_putadr.c\
		ft_puthex.c\
		ft_puthex_toupper.c\
		ft_unsigned_putnbr.c\
		ft_putchar.c\
		ft_putstr.c\
		ft_putnbr.c\
		ft_strcspn.c\
		ft_isspace.c\
		ft_strtol.c\
		getnextline/get_next_line.c\
		getnextline/get_next_line_utils.c
		
OBJ = $(SRC:%.c=%.o)
RM = rm -f
CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(LPATH)$(LIBFT) $(NAME)
	
$(NAME): $(OBJ)
	cp $(LPATH)$(LIBFT) $(NAME)
	ar -crs  $(NAME) $(OBJ)

$(LPATH)$(LIBFT):
	$(MAKE) -C $(LPATH)

clean:
	$(RM) *.o
	$(RM) $(LPATH)*.o
	$(RM) $(GPATH)*.o
fclean: clean
	$(RM) $(NAME)
	$(RM) $(LPATH)$(LIBFT)

re: fclean all

.phony: all clean fclean re