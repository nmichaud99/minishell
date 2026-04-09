/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:55:34 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 15:55:38 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			str[i] = 29;
		i++;
	}
}

void	build_one_char_variable(t_data *data, int *i, char **res, int flag)
{
	if (flag == 0)
	{
		(*i)++;
		*res = ft_strdup("");
	}
	else if (flag == 1)
	{
		(*i) = (*i) + 2;
		*res = get_variable_value(data, "?");
	}
	else if (flag == 2)
	{
		(*i) = (*i) + 2;
		*res = ft_strdup("");
	}
	else if (flag == 3)
	{
		(*i)++;
		*res = ft_strdup("$");
	}
}

char	*build_variable(t_data *data, t_word *arg, int start, int count)
{
	char	*tmp;
	char	*res;

	tmp = malloc(count + 1);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, &arg->txt[start], count);
	tmp[count] = 0;
	res = get_variable_value(data, tmp);
	free(tmp);
	return (res);
}

char	*expand_word(t_data *data, t_word *arg, int *i, t_quote_type quote)
{
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	if (quote == SPECIAL || (arg->txt[start] && arg->quoting[start] != quote))
		return (build_one_char_variable(data, i, &res, 0), res);
	if (arg->txt[start] && arg->txt[start] == '?')
		return (build_one_char_variable(data, i, &res, 1), res);
	if (arg->txt[start] && (arg->txt[start] <= '9' && arg->txt[start] >= '0'))
		return (build_one_char_variable(data, i, &res, 2), res);
	if (!(arg->txt[start] && (type_of_char(arg->txt[start],
					arg->quoting[start], quote) == 1)))
		return (build_one_char_variable(data, i, &res, 3), res);
	count = 0;
	while (arg->txt[start + count] && (type_of_char(arg->txt[start + count],
				arg->quoting[start + count], quote) >= 1))
		count++;
	res = build_variable(data, arg, start, count);
	if (quote == NONE)
		replace_spaces(res);
	*i = start + count;
	return (res);
}

char	**expand_arg(t_data *data, t_word *arg)
{
	char			**res;
	char			*tmp;
	char			*variable;
	int				i;

	i = 0;
	tmp = ft_strdup("");
	if (!tmp)
		return (NULL);
	while (arg->txt[i])
	{
		if (arg->txt[i] == '$' && arg->quoting[i] != SINGLE)
		{
			variable = expand_word(data, arg, &i, arg->quoting[i]);
			if (!variable || !append_variable(&tmp, &variable))
				return (free(tmp), NULL);
		}
		else
		{
			if (!append_char(&tmp, arg->txt[i++]))
				return (free(tmp), NULL);
		}
	}
	res = ft_split(tmp, 29);
	return (free(tmp), res);
}
