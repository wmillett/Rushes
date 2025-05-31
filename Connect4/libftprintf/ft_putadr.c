/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putadr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 17:49:23 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/10 16:13:41 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

static int	ft_count_adr(unsigned long long nb)
{
	int	i;

	i = 0;
	while (nb > 0)
	{
		nb /= 16;
		i++;
	}
	return (i);
}

static int	ft_printadr(unsigned long long nb)
{
	int	check;

	if (nb > 0)
	{
		ft_printadr(nb / 16);
		if (nb % 16 >= 10)
		{
			check = ft_putchar(((nb % 16) - 10 + 49) + 48);
			if (check == ERROR)
				return (ERROR);
		}
		else if (nb % 16 < 10)
		{
			check = ft_putchar((nb % 16) + 48);
			if (check == ERROR)
				return (ERROR);
		}
	}
	return (0);
}

int	ft_putadr(void *n)
{
	unsigned long long	nb;
	int					i;
	int					check;

	nb = (unsigned long long)n;
	check = write(1, "0x", 2);
	if (check == ERROR)
		return (ERROR);
	i = 2;
	if (nb == 0)
	{
		check = write(1, "0", 1);
		i++;
	}
	else
		check = ft_printadr(nb);
	if (check == ERROR)
		return (ERROR);
	i += ft_count_adr(nb);
	return (i);
}
