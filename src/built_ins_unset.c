/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:58:36 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/02 18:58:37 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_node(t_env **env)
{
	free((*env)->key);
	free((*env)->value);
	free(*env);
	*env = NULL;
}

void	delete_node_if(char *arg, t_data *data)
{
	t_env	*prev;
	t_env	*tmp;
	t_env	*to_delete;

	prev = NULL;
	tmp = data->env;
	while (tmp)
	{
		if (tmp && ft_strcmp(tmp->key, arg) == 0)
		{
			to_delete = tmp;
			if (prev)
				prev->next = tmp->next;
			else
				data->env = tmp->next;
			tmp = tmp->next;
			free_env_node(&to_delete);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	prev = NULL;
	tmp = data->env;
}

int	exec_unset(t_data *data, char **args)
{
	char	**tmp_args;

	if (!*(args + 1))
		return (0);
	tmp_args = args + 1;
	while (*tmp_args)
	{
		delete_node_if(*tmp_args, data);
		tmp_args++;
	}
	return (0);
}
