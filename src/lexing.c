/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:43:36 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 11:43:38 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexing(t_data *data)
{
	int		i;
	int		ret;

	i = 0;
	while (data->line[i])
	{
		while (is_space(data->line[i]))
			i++;
		if (data->line[i] == 0)
			return (1);
		else if (is_operator(data->line[i]))
		{
			ret = handle_operators(&data->tokens, data->line, &i);
			if (!ret)
				return (free_token(&data->tokens), 0);
		}
		else
		{
			ret = handle_word(&data->tokens, data->line, &i);
			if (ret <= 0)
				return (free_token(&data->tokens), word_error(ret));
		}
	}
	return (1);
}
