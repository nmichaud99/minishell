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

void	free_env_node(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
	env = NULL;
}

int	exec_unset(t_data *data, char **args)
{
	t_env	*tmp;
	t_env	*to_delete;

	tmp = data->env;
	if (!*(args + 1))
		return (0);
	while (tmp)
	{
		if (tmp && ft_strcmp(tmp->key, *(args + 1)) == 0)
		{
			to_delete = tmp;
			if (tmp->next)
				tmp = tmp->next;
			else
				tmp = NULL;
			free_env_node(to_delete);
			return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}
