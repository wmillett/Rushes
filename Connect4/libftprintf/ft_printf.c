/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 16:35:11 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/12 15:36:05 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_check_specifier(char c, va_list ap)
{
	int	len;

	len = 0;
	if (c == 'c')
		len = ft_putchar(va_arg(ap, int));
	else if (c == 's')
		len = ft_putstr(va_arg(ap, char *));
	else if (c == 'p')
		len = ft_putadr(va_arg(ap, char *));
	else if (c == 'd' || c == 'i')
		len = ft_putnbr(va_arg(ap, int));
	else if (c == 'u')
		len = ft_unsigned_putnbr(va_arg(ap, unsigned int));
	else if (c == 'x')
		len = ft_puthex(va_arg(ap, unsigned int));
	else if (c == 'X')
		len = ft_puthex_toupper(va_arg(ap, unsigned int));
	else if (c == '%')
		len = ft_putchar('%');
	else
		return (0);
	return (len);
}

int	ft_printf(const char *s, ...)
{
	int		i;
	int		len_p;
	int		check;
	va_list	ap;

	if (!s)
		return (0);
	i = 0;
	len_p = 0;
	va_start(ap, s);
	while (s[i])
	{
		check = 0;
		if (s[i] == '%')
			check = ft_check_specifier(s[++i], ap);
		else
			check = ft_putchar(s[i]);
		if (check == ERROR)
			return (ERROR);
		len_p += check;
		i++;
	}
	va_end(ap);
	return (len_p);
}
