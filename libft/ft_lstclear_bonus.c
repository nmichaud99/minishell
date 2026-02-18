/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:37:39 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/17 11:56:11 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*list;
	t_list	*temp;

	if (*lst == NULL || del == NULL)
	{
		list = NULL;
		temp = NULL;
	}
	else
	{
		list = *lst;
		while (list -> next != NULL)
		{
			temp = list;
			list = list -> next;
			ft_lstdelone(temp, del);
		}
		ft_lstdelone(list, del);
		*lst = NULL;
	}
}
