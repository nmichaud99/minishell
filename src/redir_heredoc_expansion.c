/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:54:32 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/24 17:54:34 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_exit_code(int *i, int start, t_data *data)
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

char	*expand_variable(t_data *data, char *line, int *i)
{
	char	*tmp;
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	if (line[start] == '?')
	{
/* 		(*i) = start + 1;
		tmp = "?";
		res = get_variable_value(data, tmp);
		if (!res)
			return (NULL);
		return (res); */
		return (expand_exit_code(i, start, data));
	}
	else if (line[start] && (line[start] <= '9' && line[start] >= '0'))
	{
		(*i) = start + 1;
		res = ft_strdup("");
		if (!res)
			return (NULL);
		return (res);
	}
	else if (!(line[start] && ((line[start] <= 'z' && line[start] >= 'a') || (line[start] <= 'Z' && line[start] >= 'A')
		|| line[start] == '_')))
	{
		(*i)++;
		res = ft_strdup("$");
		if (!res)
			return (NULL);
		return (res);
	}
	count = 1;
	while (line[start + count] && ((line[start + count] <= 'z' && line[start + count] >= 'a') || (line[start + count] <= 'Z' && line[start + count] >= 'A')
		|| line[start + count] == '_' || (line[start + count] <= '9' && line[start + count] >= '0')))
		count++;
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
