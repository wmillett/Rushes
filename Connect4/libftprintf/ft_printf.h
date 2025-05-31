/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:21:13 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/12 15:31:55 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft/libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include "getnextline/get_next_line.h"

# define ERROR -1

int	ft_putadr(void *n);

int	ft_printf(const char *s, ...);

int	ft_unsigned_putnbr(unsigned int n);

int	ft_puthex(unsigned int nb);

int	ft_puthex_toupper(unsigned int nb);

// Wrapper functions for libft _fd functions
int	ft_putchar(char c);
int	ft_putstr(char *s);
int	ft_putnbr(int n);

// Additional utility functions for Connect4
size_t	ft_strcspn(const char *s1, const char *s2);
int		ft_isspace(int c);
long	ft_strtol(const char *str, char **endptr, int base);

char		*get_next_line(int fd);
void		cleanup_get_next_line(void);

#endif