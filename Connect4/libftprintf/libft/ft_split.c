/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 16:35:49 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/24 18:28:30 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if ((s[i] != c) && ((s[i + 1] == c) || (s[i + 1] == 0)))
			count++;
		i++;
	}
	return (count);
}

static int	free_memory(char **tab, int j, int words)
{
	while (j >= 0)
	{
		free(tab[j]);
		j--;
	}
	free(tab[words]);
	free(tab);
	return (1);
}

static int	allocation(char **tab, char const *s, char c, int words)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	start = 0;
	while (s[i])
	{
		if ((s[i] != c) && (i == 0))
			start = i;
		else if ((s[i] != c) && (s[i - 1] == c))
			start = i;
		if ((s[i] != c) && ((s[i + 1] == c) || (s[i + 1] == 0)))
		{
			tab[j] = ft_substr(s, start, i - start + 1);
			if (tab[j] == NULL)
				return (free_memory(tab, j - 1, words));
			j++;
		}
		i++;
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		words;
	int		result;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	tab = (char **)malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	tab[words] = NULL;
	result = allocation(tab, s, c, words);
	if (result == 1)
		return (NULL);
	return (tab);
}
