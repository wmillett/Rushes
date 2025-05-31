/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 15:09:38 by wmillett          #+#    #+#             */
/*   Updated: 2023/01/24 18:31:31 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	if (!s)
		return (0);
	i = ft_strlen(s);
	while (1)
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		if (i == 0)
			break ;
		i--;
	}
	return (NULL);
}
