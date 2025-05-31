/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 21:47:36 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/27 23:06:14 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*p;

	if (!del)
		return ;
	while (*lst != NULL)
	{
		p = (*lst)->next;
		(*del)((*lst)->content);
		free(*lst);
		*lst = p;
	}
}
