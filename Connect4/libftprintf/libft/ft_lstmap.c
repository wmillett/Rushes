/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 23:09:38 by wmillett          #+#    #+#             */
/*   Updated: 2023/01/30 14:35:38 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new;    
	void	*content;   

	if ((!f) || (!del))
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		content = ((*f)(lst->content));
		new = ft_lstnew(content);
		if (!new)
		{
			free(content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new);
		lst = lst->next;
	}
	return (new_lst);
}
//dsfds
