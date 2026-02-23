/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:29:17 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/23 16:29:19 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_pipes(t_data *data)
{
	if (!data)
		return ;
	while (data->tokens != NULL)
	{
		if (data->tokens->type == PIPE)
			add_token(&nodes, new(node, 
	}
}
