/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:31:39 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/23 19:42:00 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 <files>.c

#include "get_next_line.h"

// Static variable to hold leftover input between calls
static char *g_leftover = NULL;

size_t	ft_find(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_small_split(char *line, size_t *pos)
{
	char	*fnext_line;
	size_t	i;

	i = 0;
	while (line[i])
		if (line[i++] == '\n')
			break ;
	*pos = i;
	fnext_line = ft_calloc(i + 1, sizeof(char));
	if (!fnext_line)
		return (ft_sfree(fnext_line));
	while (i--)
		fnext_line[i] = line[i];
	return (fnext_line);
}

// Custom strjoin that frees s1 (compatible with get_next_line usage)
char *gnl_strjoin(char *s1, char *s2)
{
	char *result;
	
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
	{
		s1 = ft_calloc(1, sizeof(char));
		if (!s1)
			return (NULL);
	}
	
	result = ft_strjoin(s1, s2);
	ft_sfree(s1);  // Free the old s1
	return (result);
}

char	*get_next_line(int fd)
{
	t_var		var;

	// Simplified validation - removed OPEN_MAX check
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (g_leftover = ft_sfree(g_leftover));
	if (!g_leftover)
		g_leftover = ft_calloc(1, sizeof(char));
	var.buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	var.pos = 0;
	while (ft_find(g_leftover) == 0)
	{
		ft_bzero(var.buffer, BUFFER_SIZE + 1);
		var.rd = read(fd, var.buffer, BUFFER_SIZE);
		if (var.rd <= 0)
			break ;
		g_leftover = gnl_strjoin(g_leftover, var.buffer);
	}
	var.buffer = ft_sfree(var.buffer);
	if (var.rd == -1 || (var.rd <= 0 && *g_leftover == 0))
		return (g_leftover = ft_sfree(g_leftover), NULL);
	var.next_line = ft_small_split(g_leftover, &var.pos);
	var.buffer = g_leftover;
	g_leftover = gnl_strjoin(NULL, g_leftover + var.pos);
	return (ft_sfree(var.buffer), var.next_line);
}

// Cleanup function to free static memory used by get_next_line
void cleanup_get_next_line(void)
{
	if (g_leftover)
	{
		ft_sfree(g_leftover);
		g_leftover = NULL;
	}
}
