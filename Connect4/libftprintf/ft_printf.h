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

int	ft_putadr(void *n);

int	ft_printf(const char *s, ...);

int	ft_unsigned_putnbr(unsigned int n);

int	ft_puthex(unsigned int nb);

int	ft_puthex_toupper(unsigned int nb);

#endif