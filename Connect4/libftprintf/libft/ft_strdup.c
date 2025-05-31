/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 17:00:37 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/24 18:29:23 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	len;
	size_t	i;

	if (!s1)
		return (0);
	len = ft_strlen(s1);
	i = 0;
	copy = (char *)malloc(sizeof(char) * len + 1);
	if (!copy)
		return (NULL);
	while (i < len + 1)
	{
		copy[i] = s1[i];
		i++;
	}
	return (copy);
}
