/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 18:31:35 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/12 15:36:55 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

static int	ft_hexalen(unsigned int nb)
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

static int	ft_printhexa(unsigned int nb)
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
		i = ft_putchar((nb - 10 + 'a'));
		if (i == ERROR)
			return (ERROR);
	}
	return (0);
}

static int	ft_puthex2(unsigned int nb)
{
	int	i;

	if (nb >= 16)
	{
		i = ft_puthex2(nb / 16);
		if (i == ERROR)
			return (ERROR);
		i = ft_puthex2(nb % 16);
		if (i == ERROR)
			return (ERROR);
	}
	else
	{
		i = ft_printhexa(nb);
		if (i == ERROR)
			return (ERROR);
	}
	return (ft_hexalen(nb));
}

int	ft_puthex(unsigned int nb)
{
	int	i;

	if (nb == 0)
	{
		i = ft_putchar(48);
		if (i == ERROR)
			return (ERROR);
		return (1);
	}
	i = ft_puthex2(nb);
	if (i == ERROR)
		return (ERROR);
	return (i);
}
