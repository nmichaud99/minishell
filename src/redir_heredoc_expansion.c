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

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_alpha(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
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
