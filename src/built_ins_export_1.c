/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_export_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:56:38 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 12:56:40 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_variable_key(const char *s)
{
	char	*result;
	int		i;
	int		len;

	len = 0;
	while (s[len] && s[len] != '=')
		len++;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

void	print_env_export(t_data *data)
{
	t_env	*cpy;
	t_env	*first;
	int		swapped;
	char	*tmp_key;
	char	*tmp_value;

	first = data->env;
	cpy = data->env;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		cpy = first;
		while (cpy && cpy->next)
		{
			if (ft_strcmp(cpy->key, cpy->next->key) > 0)
			{
				tmp_key = cpy->key;
				tmp_value = cpy->value;
				cpy->key = cpy->next->key;
				cpy->value = cpy->next->value;
				cpy->next->key = tmp_key;
				cpy->next->value = tmp_value;
				swapped = 1;
			}
			cpy = cpy->next;
		}
	}
	while (first)
	{
		printf("export ");
		printf("%s", first->key);
		if (first->has_value)
			printf("=\"%s\"\n", first->value);
		else
			printf("\n");
		first = first->next;
	}
}

int	is_valid_string(char *str, int has_value)
{
	int	i;

	i = 0;
	if (str[0] && ((str[i] <= 'z' && str[i] >= 'a')
			|| (str[i] <= 'Z' && str[i] >= 'A') || str[i] == '_'))
		i++;
	else
		return (0);
	while (str[i] && str[i] != '=')
	{
		if ((str[i] <= 'z' && str[i] >= 'a')
			|| (str[i] <= 'Z' && str[i] >= 'A')
			|| (str[i] <= '9' && str[i] >= '0') || str[i] == '_')
				i++;
		else
			return (0);
	}
	if (has_value && str[i] != '=')
		return (0);
	if (!has_value && str[i] == '=')
		return (0);
	return (1);
}

int	find_key(t_data *data, char *key)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	ft_schr(const char *s, char c)
{
	int		i;
	char	c1;

	c1 = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == c1)
			return (1);
		i++;
	}
	return (0);
}
