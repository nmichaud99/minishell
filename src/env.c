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

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if ((unsigned char)s1[i] == (unsigned char)s2[i])
			i++;
		else
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	}
	return (0);
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

// Exec export
void	add_or_modify_env_node(t_data *data, char *new_var)
{
	t_env	*tmp;
	char	*new_key;
	char	*new_value;

	/*if (!is_valid_string(new_var))
		return ;*/
	new_key = get_variable_key(new_var);
	if (!new_key)
		exit_free(data, EXIT_FAILURE);
	new_value = ft_strdup(ft_strchr(new_var, '=') + 1);
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

int	expansion(t_data *data)
{
	t_cmd_list	*tmp_list;
	char		**tmp_args;

	tmp_list = data->cmd_list;
	tmp_args = tmp_list->args;
	if (*tmp_args)
	{
		if (ft_strlen(*tmp_args) == 6 && ft_strcmp(*tmp_args, "export") == 0)
		{
			if (!*(tmp_args + 1))
			{
				print_env_export(data);
				return (1);
			}
			else if (!is_valid_string(*(tmp_args + 1)))
			{
				printf("export: '%s': is not a valid identifier\n", *(tmp_args + 1));
					return (2);
			}
			add_or_modify_env_node(data, *(tmp_args + 1));
		}
	}
	return (0);
}

// Free env node
void	free_env_node(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
	env = NULL;
}

// Exec Unset command

int	exec_unset(t_data *data, char *var)
{
	t_env	*tmp;
	t_env	*to_delete;

	tmp = data->env;
	while (tmp)
	{
		if (tmp->next && ft_strcmp(tmp->next->key, var) == 0)
		{
			to_delete = tmp->next;
			if (tmp->next->next)
				tmp->next = tmp->next->next;
			else
				tmp->next = NULL;
			free_env_node(to_delete);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
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
