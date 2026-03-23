/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:05:43 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 12:05:52 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_exit_code(t_data *data)
{
	char	*tmp_value;

	tmp_value = ft_itoa(data->exit_status);
	if (!tmp_value)
		return (NULL);
	return (tmp_value);
}

char	*get_variable_value(t_data *data, char *str)
{
	t_env	*tmp;
	char	*tmp_value;

	if (ft_strcmp(str, "?") == 0)
		return (get_exit_code(data));
	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, str) == 0)
		{
			tmp_value = ft_strdup(tmp->value);
			if (!tmp_value)
				return (NULL);
			return (tmp_value);
		}
		tmp = tmp->next;
	}
	tmp_value = ft_strdup("");
	if (!tmp_value)
		return (NULL);
	return (tmp_value);
}

int	type_of_char(char c, t_quote_type quoting, t_quote_type quote)
{
	if (c && !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
		&& quoting == quote)
		return (1);
	else if (c && ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
			|| (c >= '0' && c <= '9') || c == '_')
		&& quoting == quote)
		return (2);
	return (0);
}

int	append_variable(char **res, char **str)
{
	char	*new_str;
	int		size;
	int		size2;

	size = ft_strlen(*res);
	size2 = ft_strlen(*str);
	new_str = malloc(size + size2 + 1);
	if (!new_str)
		return (0);
	ft_memcpy(new_str, *res, size);
	ft_memcpy(new_str + size, *str, size2);
	new_str[size + size2] = 0;
	free(*res);
	free(*str);
	*res = new_str;
	return (1);
}

int	append_char(char **res, char c)
{
	char	*new_str;
	int		size;

	size = ft_strlen(*res);
	new_str = malloc(size + 2);
	if (!new_str)
		return (0);
	ft_memcpy(new_str, *res, size);
	new_str[size] = c;
	new_str[size + 1] = 0;
	free(*res);
	*res = new_str;
	return (1);
}
