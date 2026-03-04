/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:03:34 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/02 19:03:37 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// recuperer la value (...=VALUE) pour chaque ligne des variables env
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

void	add_env_node(t_data *data, char *env_line)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		exit_free(data, EXIT_FAILURE);
	new_node->key = get_variable_key(env_line);
	if (!new_node->key)
		exit_free(data, EXIT_FAILURE);
	new_node->value = ft_strdup(ft_strchr(env_line, '=') + 1);
	if (!new_node->value)
	{
		free(new_node->key);
		new_node->key = NULL;
		free(new_node);
		exit_free(data, EXIT_FAILURE);
	}
	new_node->next = NULL;
	tmp = data->env;
	if (!data->env)
	{
		data->env = new_node;
		return ;
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	add_or_modify_env_node(t_data *data, char *new_var)
{
	t_env	*tmp;
	char	*new_key;
	char	*new_value;

	new_key = get_variable_key(new_var);
	if (!new_key)
		exit_free(data, EXIT_FAILURE);
	new_value = ft_strdup(ft_strchr(new_var, '=') + 1);
	printf("new_value : %s\n", new_value);
	if (!new_value)
	{
		free(new_key);
		exit_free(data, EXIT_FAILURE);
	}
	tmp = data->env;
	while (tmp && tmp->next)
	{
		if (ft_strcmp(tmp->key, new_key) == 0)
		{
			if (ft_strcmp(new_value, tmp->value) == 0)
				return ;
			else
			{
				tmp->value = new_value;
				return ;
			}
		}
		tmp = tmp->next;
	}
	free(new_key);
	free(new_value);
	add_env_node(data, new_var);
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
	if (str[0] && ((str[i] <= 'z' && str[i] >= 'a') || (str[i] <= 'Z' && str[i] >= 'A') || str[i] == '_'))
		i++;
	else
		return (0);
	while (str[i] && str[i] != '=')
	{
		if ((str[i] <= 'z' && str[i] >= 'a') || (str[i] <= 'Z' && str[i] >= 'A') || (str[i] <= '9' && str[i] >= '0') || str[i] == '_')
				i++;
		else
			return (0);
	}
	if (!str[i] || str[i] != '=')
		return (0);
	return (1);
}

int	ft_export(t_data *data, char **args)
{
	if (!*(args + 1))
	{
		print_env_export(data);
		return (1);
	}
	else
	{
		if (!is_valid_string(*(args + 1)))
		{
			printf("export: '%s': is not a valid identifier\n", *(args + 1));
				return (2);
		}
		add_or_modify_env_node(data, *(args + 1));
	}
	return (0);
}
