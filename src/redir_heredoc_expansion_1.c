/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc_expansion_1.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:49:39 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 15:49:42 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_exit_code(int *i, int start, t_data *data)
{
	char	*tmp;
	char	*res;

	(*i) = start + 1;
	tmp = "?";
	res = get_variable_value(data, tmp);
	if (!res)
		return (NULL);
	return (res);
}

char	*handle_digit(int *i, int start)
{
	char	*res;

	(*i) = start + 1;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	return (res);
}

char	*handle_invalid(int *i)
{
	char	*res;

	(*i)++;
	res = ft_strdup("$");
	if (!res)
		return (NULL);
	return (res);
}

int	valid_count(char *str, int start)
{
	int	i;

	i = 1;
	while (str[start + i] && (is_alpha(str[start + i]) || str[start + i] == '_'
			|| is_digit(str[start + i])))
		i++;
	return (i);
}
