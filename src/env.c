/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:11:50 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 15:11:50 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_env_tab(char **env, t_data *data)
{
	int	i;

	i = 0;
	while (env[i])
	{
		add_env_node(data, env[i]);
		i++;
	}
}

// Exec env
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

char	*get_variable_value(t_data *data, char *str)
{
	t_env	*tmp;
	char	*tmp_value;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, str) == 0)
		{
			tmp_value = ft_strdup(tmp->value);
			if (!tmp_value)
				exit_free(data, EXIT_FAILURE);
			return (tmp_value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
