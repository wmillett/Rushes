/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:43:30 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/24 18:32:37 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	create_str(char const *s, char *new_str, unsigned int start,
		size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[start + j])
	{
		if (i < len)
		{
			new_str[i] = s[start + i];
			i++;
		}
		j++;
	}
	new_str[i] = '\0';
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	m_size;

	if (!s)
		return (0);
	if (start >= ft_strlen(s))
	{
		len = 0;
		m_size = 0;
	}
	m_size = ft_strlen(s) - start;
	if (len < m_size)
		m_size = len;
	new_str = (char *)malloc(sizeof(char) * (m_size + 1));
	if (!new_str)
		return (NULL);
	if (m_size == 0)
	{
		*new_str = 0;
		return (new_str);
	}
	create_str(s, new_str, start, len);
	return (new_str);
}
