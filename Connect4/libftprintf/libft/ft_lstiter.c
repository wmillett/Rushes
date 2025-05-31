/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmillett <wmillett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 22:40:30 by wmillett          #+#    #+#             */
/*   Updated: 2023/01/27 23:07:38 by wmillett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if ((!lst) || (!f))
		return ;
	while (lst)
	{
		(*f)(lst->content);
		lst = lst->next;
	}
}
