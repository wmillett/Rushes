/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 21:38:17 by wmillett          #+#    #+#             */
/*   Updated: 2023/01/24 18:28:45 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;
	int	size;

	i = 0;
	if (!s)
		return (0);
	size = ft_strlen(s);
	while (i <= size)
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	return (NULL);
}
