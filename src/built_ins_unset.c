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

int	exec_unset(t_data *data, char **args)
{
	t_env	*tmp;
	t_env	*prev;
	t_env	*to_delete;

	tmp = data->env;
	prev = NULL;
	if (!*(args + 1))
		return (0);
	while (tmp)
	{
		if (tmp && ft_strcmp(tmp->key, *(args + 1)) == 0)
		{
			to_delete = tmp;
			if (prev)
				prev->next = tmp->next;
			else
				data->env = tmp->next;
			tmp = tmp->next;
			free_env_node(&to_delete);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (0);
}
