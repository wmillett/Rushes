/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned_putnbr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 20:02:55 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/10 16:25:12 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

static int	ft_unsigned_numlen(unsigned long n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

int	ft_unsigned_putnbr(unsigned int n)
{
	int	len;
	int	check;

	len = ft_unsigned_numlen(n);
	if (n >= 0 && n < 10)
	{
		check = ft_putchar(n + 48);
		if (check == ERROR)
			return (ERROR);
	}
	else
	{
		check = ft_putnbr(n / 10);
		if (check == ERROR)
			return (ERROR);
		check = ft_putnbr(n % 10);
		if (check == ERROR)
			return (ERROR);
	}
	return (len);
}
