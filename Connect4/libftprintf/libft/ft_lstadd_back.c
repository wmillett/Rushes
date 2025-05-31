/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 16:51:29 by wmillett          #+#    #+#             */
/*   Updated: 2023/01/27 23:05:56 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*p;

	if (!new)
		return ;
	p = ft_lstlast(*lst);
	if ((p) && (p != new))
		p->next = new;
	else if (!p)
		*lst = new;
}
