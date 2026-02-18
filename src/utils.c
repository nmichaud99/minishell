/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:40:16 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 16:40:17 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_token(t_token **head)
{
	t_token *tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		free((*head)->str);
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
		tmp = NULL;
	}
	head = NULL;
}
