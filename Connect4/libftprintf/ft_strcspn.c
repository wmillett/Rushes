/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 16:00:00 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/12 16:00:00 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_strcspn(const char *s1, const char *s2)
{
	size_t	len;
	size_t	i;

	len = 0;
	while (s1[len])
	{
		i = 0;
		while (s2[i])
		{
			if (s1[len] == s2[i])
				return (len);
			i++;
		}
		len++;
	}
	return (len);
} 