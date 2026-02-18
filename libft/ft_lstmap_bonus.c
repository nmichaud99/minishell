/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:25:18 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/17 15:07:27 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	t_list	*delete(t_list *res, void (*del)(void *), void *content)
{
	del(content);
	ft_lstclear(&res, del);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*res;
	void	*content;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	res = NULL;
	while (lst != NULL)
	{
		content = f(lst -> content);
		if (!content)
		{
			ft_lstclear(&res, del);
			return (NULL);
		}
		new = ft_lstnew(content);
		if (!new)
			return (delete(res, del, content));
		ft_lstadd_back(&res, new);
		lst = lst -> next;
	}
	return (res);
}
