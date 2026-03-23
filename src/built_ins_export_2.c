/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_export_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:56:31 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 12:56:32 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_node(t_env **new_node, char *env_line)
{
	t_env	*node;

	node = *new_node;
	node->key = get_variable_key(env_line);
	if (!*node->key)
		return (0);
	node->value = ft_strdup(ft_strchr(env_line, '=') + 1);
	if (!node->value)
	{
		free(node->key);
		node->key = NULL;
		free(node);
		return (0);
	}
	node->next = NULL;
	return (1);
}

int	add_env_node(t_data *data, char *env_line)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (0);
	if (!create_node(&new_node, env_line))
		return (0);
	tmp = data->env;
	if (!data->env)
	{
		data->env = new_node;
		return (1);
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (1);
}

int	loop_over_each_node(t_data *data, char *new_key, char *new_value)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, new_key) == 0)
		{
			if (ft_strcmp(new_value, tmp->value) == 0)
				return (free(new_key), free(new_value), 1);
			else
			{
				free(tmp->value);
				tmp->value = new_value;
				free(new_key);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	free(new_key);
	free(new_value);
	return (0);
}

int	add_or_modify_env_node(t_data *data, char *new_var)
{
	char	*new_key;
	char	*new_value;

	new_key = get_variable_key(new_var);
	if (!new_key)
		return (0);
	new_value = ft_strdup(ft_strchr(new_var, '=') + 1);
	if (!new_value)
	{
		free(new_key);
		return (0);
	}
	if (loop_over_each_node(data, new_key, new_value))
		return (1);
	if (!add_env_node(data, new_var))
		return (0);
	return (1);
}

int	exec_export(t_data *data, char **args)
{
	if (!*(args + 1))
	{
		print_env_export(data);
		return (1);
	}
	else
	{
		args++;
		while (*args)
		{
			if (!is_valid_string(*args))
			{
				printf("export: '%s': is not a valid identifier\n", *args);
				return (2);
			}
			if (!add_or_modify_env_node(data, *args))
				return (1);
			args++;
		}
	}
	return (0);
}
