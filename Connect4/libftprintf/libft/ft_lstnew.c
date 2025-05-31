/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouellet <aouellet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:41:33 by aouellet          #+#    #+#             */
/*   Updated: 2023/01/26 19:29:43 by aouellet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_lst;

	new_lst = (t_list *)malloc(sizeof(t_list));
	if (!new_lst)
		return (NULL);
	if (!content)
		new_lst->content = NULL;
	else
		new_lst->content = content;
	new_lst->next = NULL;
	return (new_lst);
}
