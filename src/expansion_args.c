/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:07:36 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 12:07:46 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_word(t_data *data, t_word *arg, int *i, t_quote_type quote)
{
	char	*tmp;
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	count = 0;
	if (type_of_char(arg->txt[start], arg->quoting[start], quote) == 1)
		count++;
	while (type_of_char(arg->txt[start + count],
			arg->quoting[start + count], quote) == 2)
		count++;
	tmp = malloc(count + 1);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, &arg->txt[start], count);
	tmp[count] = 0;
	res = get_variable_value(data, tmp);
	free(tmp);
	if (!res)
		return (NULL);
	*i = start + count;
	return (res);
}

char	*expand_arg(t_data *data, t_word *arg)
{
	char			*res;
	char			*variable;
	int				i;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (arg->txt[i])
	{
		if (arg->txt[i] == '$' && arg->quoting[i] != SINGLE)
		{
			variable = expand_word(data, arg, &i, arg->quoting[i]);
			if (!variable || !append_variable(&res, &variable))
				return (free(res), NULL);
		}
		else
		{
			if (!append_char(&res, arg->txt[i]))
				return (free(res), NULL);
			i++;
		}
	}
	return (res);
}

char	**get_expanded_args(t_data *data, t_cmd_list *lst)
{
	char	**ret;
	int		i;
	int		size;

	size = 0;
	while (lst->args[size])
		size++;
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (i < size + 1)
		ret[i++] = NULL;
	i = 0;
	while (lst->args[i])
	{
		ret[i] = expand_arg(data, lst->args[i]);
		if (!ret[i])
		{
			ft_free(&ret);
			return (NULL);
		}
		i++;
	}
	return (ret);
}
