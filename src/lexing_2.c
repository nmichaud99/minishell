/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 18:17:13 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 18:17:15 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '\\')
		return (1);
	return (0);
}

int	dollar_exists(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	backslash_exists(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			return (1);
		i++;
	}
	return (0);
}

int	is_backslash_char(char c)
{
	if (c == 'a' || c == 'b' || c == 't' || c == 'n' || c == 'v'
		|| c == 'f' || c == 'r')
		return (1);
	return (0);
}
