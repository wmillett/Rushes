/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 19:52:50 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/24 18:29:58 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dst_size;
	size_t	src_size;

	if ((!dst) || (!src))
		return (0);
	dst_size = ft_strlen(dst);
	src_size = ft_strlen(src);
	if (dstsize <= dst_size)
		return (src_size + dstsize);
	i = dst_size;
	j = 0;
	while ((src[j] != '\0') && (i + j < (dstsize - 1)))
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst_size + src_size);
}
