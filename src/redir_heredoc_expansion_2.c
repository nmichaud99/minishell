/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc_expansion_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:49:45 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 15:49:47 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variable(t_data *data, char *line, int *i)
{
	char	*tmp;
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	if (line[start] == '?')
		return (handle_exit_code(i, start, data));
	else if (line[start] && is_digit(line[start]))
		return (handle_digit(i, start));
	else if (!(line[start] && (is_alpha(line[start]) || line[start] == '_')))
		return (handle_invalid(i));
	count = valid_count(line, start);
	tmp = malloc(count + 1);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, &line[start], count);
	tmp[count] = 0;
	res = get_variable_value(data, tmp);
	free(tmp);
	if (!res)
		return (NULL);
	*i = start + count;
	return (res);
}

char	*expand_line(t_data *data, char *line)
{
	int		i;
	char	*res;
	char	*variable;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			variable = expand_variable(data, line, &i);
			if (!variable || !append_variable(&res, &variable))
				return (free(res), NULL);
		}
		else
		{
			if (!append_char(&res, line[i]))
				return (free(res), NULL);
			i++;
		}
	}
	return (res);
}
