/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 15:10:29 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/24 18:26:38 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_int_size(long n)
{
	int	size;

	size = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static int	check_if_neg(long *n)
{
	int	result;

	result = 0;
	if (*n < 0)
	{
		result = 1;
		*n *= -1;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	int		i;
	int		size;
	int		is_it_neg;
	char	*str;
	long	nb;

	nb = (long)n;
	is_it_neg = check_if_neg(&nb);
	size = get_int_size(nb);
	str = (char *)malloc(sizeof(char) * (size + is_it_neg + 1));
	if (!str)
		return (NULL);
	i = size - 1;
	str[0] = '-';
	while (i >= 0)
	{
		str[i + is_it_neg] = nb % 10 + 48;
		nb = nb / 10;
		i--;
	}
	str[size + is_it_neg] = 0;
	return (str);
}
