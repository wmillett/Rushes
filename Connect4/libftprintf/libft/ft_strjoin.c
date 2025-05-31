/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:22:30 by aouellet          #+#    #+#             */
/*   Updated: 2023/12/12 17:51:24 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	size;

	if(!s1 && s2)
		return (ft_strdup(s2));
	else if(!s1 || !s2)
		return NULL;
	size = ft_strlen(s1) + ft_strlen(s2);
	new_str = (char *)malloc(sizeof(char) * (size + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, size + 1);
	ft_strlcat(new_str, s2, size + 1);
	return (new_str);
}
