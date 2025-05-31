/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex_toupper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:49:01 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/12 15:37:00 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_hexalen_to(unsigned int nb)
{
	int	i;

	i = 0;
	while (nb)
	{
		nb /= 16;
		i++;
	}
	return (i);
}

static int	ft_printhexa_to(unsigned int nb)
{
	int	i;

	if (nb <= 9)
	{
		i = ft_putchar((nb + '0'));
		if (i == ERROR)
			return (ERROR);
	}
	else
	{
		i = ft_putchar((nb - 10 + 'A'));
		if (i == ERROR)
			return (ERROR);
	}
	return (0);
}

static int	ft_puthex_to(unsigned int nb)
{
	int	i;

	if (nb >= 16)
	{
		i = ft_puthex_to(nb / 16);
		if (i == ERROR)
			return (ERROR);
		i = ft_puthex_to(nb % 16);
		if (i == ERROR)
			return (ERROR);
	}
	else
	{
		i = ft_printhexa_to(nb);
		if (i == ERROR)
			return (ERROR);
	}
	return (ft_hexalen_to(nb));
}

int	ft_puthex_toupper(unsigned int nb)
{
	int	i;

	if (nb == 0)
	{
		i = ft_putchar(48);
		if (i == ERROR)
			return (ERROR);
		return (1);
	}
	i = ft_puthex_to(nb);
	if (i == ERROR)
		return (ERROR);
	return (i);
}
