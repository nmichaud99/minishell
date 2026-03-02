/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:40:56 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/27 12:40:57 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(t_data *data)
{
	t_token	*tmp;

	tmp = data->tokens;
	if (!tmp)
		return (1);
	if (tmp->type == PIPE)
		return (0); // error pipe in first position
	else
		tmp = tmp->next;
	while (tmp)
	{
		if (tmp->type == PIPE && tmp->next && tmp->next->type == PIPE)
			return (0); // error pipe after pipe
		else if (tmp->type == PIPE && !tmp->next)
			return (0); // error pipe in last position
		else if (is_redir(tmp->type) && tmp->next && is_redir(tmp->next->type))
			return (0); // error double redir
		else if (is_redir(tmp->type) && tmp->next && tmp->next->type == PIPE)
			return (0); // error pipe after redir
		else if (is_redir(tmp->type) && !tmp->next)
			return (0); // error nothing after redir
		tmp = tmp->next;
	}
	return (1); // correct syntax
}
