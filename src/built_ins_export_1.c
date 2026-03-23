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
	t_env	*tmp;

	tmp = data->env;
	while (tmp && tmp->next)
	{
		printf("export ");
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	printf("%s=%s\n", tmp->key, tmp->value);
}

int	is_valid_string(char *str)
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
	if (!str[i] || str[i] != '=')
		return (0);
	return (1);
}
