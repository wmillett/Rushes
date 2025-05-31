/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:31:42 by wmillett          #+#    #+#             */
/*   Updated: 2023/02/23 19:44:34 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"

//struct
typedef struct s_var
{
	char	*buffer;
	char	*next_line;
	int		rd;
	size_t	pos;
}			t_var;

//prototype - only functions not in libft
void		*ft_sfree(void *str);
char		*get_next_line(int fd);
size_t		ft_find(char *str);
void		cleanup_get_next_line(void);

#endif