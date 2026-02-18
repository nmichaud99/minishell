/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 12:34:30 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/14 13:07:02 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*list;

	if (lst == NULL)
		return (0);
	list = lst;
	i = 1;
	while (list -> next != NULL)
	{
		list = list -> next;
		i++;
	}
	return (i);
}
