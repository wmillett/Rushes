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
SRC = ft_printf.c\
		ft_putadr.c\
		ft_puthex.c\
		ft_puthex_toupper.c\
		ft_unsigned_putnbr.c\
		
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
fclean:
	$(RM) *.o
	$(RM) $(NAME)
	$(RM) $(LPATH)*.o
	$(RM) $(LPATH)$(LIBFT)

re: fclean all

.phony: all clean fclean re