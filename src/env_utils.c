/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 19:27:30 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/04 19:27:41 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_data *data)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp && tmp->next)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	printf("%s=%s\n", tmp->key, tmp->value);
}

char	**get_env_tab(t_data *data)
{
	t_env	*tmp;
	char	**res;
	char	*tmp_str;
	int		i;

	tmp = data->env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	tmp = data->env;
	while (tmp)
	{
		tmp_str = ft_strjoin(tmp->key, "=");
		if (!tmp_str)
			return (NULL);
		res[i] = ft_strjoin(tmp_str, tmp->value);
		free(tmp_str);
		if (!res[i])
		{
			ft_free(&res);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	res[i] = NULL;
	return (res);
}
