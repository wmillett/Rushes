/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:02:15 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/30 10:03:50 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	_check(char const *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_str;
	int		start;
	int		end;
	int		i;

	if ((!s1) || (!set))
		return (NULL);
	i = 0;
	while ((s1[i]) && (_check(set, s1[i]) == 1))
		i++;
	start = i;
	i = (int)ft_strlen(s1) - 1;
	end = i;
	while ((i > 0) && (_check(set, s1[i]) == 1))
		i--;
	end = i + 1;
	i = end - start;
	if ((start == (int)ft_strlen(s1)) || (end == 0))
		i = 0;
	new_str = (char *)malloc(sizeof(char) * (i + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1 + start, i + 1);
	return (new_str);
}
