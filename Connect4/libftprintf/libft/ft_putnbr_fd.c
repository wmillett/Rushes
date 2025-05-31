/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 16:12:42 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/26 16:29:23 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	tab[10];
	long	nb;
	int		i;

	nb = n;
	if (nb == 0)
		write(fd, "0", 1);
	if (nb < 0)
	{
		write(fd, "-", 1);
		nb *= -1;
	}
	i = 0;
	while (nb)
	{
		tab[i] = nb % 10 + 48;
		i++;
		nb /= 10;
	}
	i--;
	while (i >= 0)
	{
		write(fd, &tab[i], 1);
		i--;
	}
}
