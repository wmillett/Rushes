/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 18:07:14 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/24 18:30:57 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if ((!haystack) || (!needle))
		return (0);
	if (needle[0] == '\0')
		return ((char *)haystack);
	if (*haystack == '\0')
		return (NULL);
	while ((haystack[i] != '\0') && (i < len))
	{
		j = 0;
		while ((haystack[j + i] == needle[j]) && (i + j < len))
		{
			if (needle[j + 1] == '\0')
				return ((char *)haystack + i);
			j++;
		}
		i++;
	}
	return (NULL);
}
