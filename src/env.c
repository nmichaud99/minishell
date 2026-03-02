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
	if (str[0] && str[0] == '=')
		return (0);
	if (!ft_strchr(str, '=') || !(ft_strchr(str, '=') + 1))
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
		return ;
	new_node->key = get_variable_key(env_line);
	new_node->value = ft_strdup(ft_strchr(env_line, '=') + 1);
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


// Exec export
void	add_or_modify_env_node(t_data *data, char *new_var)
{
	t_env	*tmp;
	char	*new_key;
	char	*new_value;

	if (!is_valid_string(new_var))
		return ;
	new_key = get_variable_key(new_var);
	new_value = ft_strdup(ft_strchr(new_var, '=') + 1);
	if (!new_value)
		return ;
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
	add_env_node(data, new_var);
}

// Free env node
void	free_env_node(t_env *env)
{
	free(env->key);
	free(env->value);
}

// Exec Unset command

void	exec_unset(t_data *data, char *var)
{
	t_env	*tmp;
	t_env	*to_delete;

	tmp = data->env;
	while (tmp && tmp->next)
	{
		if (tmp->next && ft_strcmp(tmp->next->key, var) == 0)
		{
			to_delete = tmp->next;
			if (tmp->next->next)
				tmp->next = tmp->next->next;
			else
				tmp->next = NULL;
			free_env_node(to_delete);
			printf("Variable %s successfully deleted from env!\n", var);
			return ;
		}
		tmp = tmp->next;
	}
	printf("The variable %s does not exist in the env !\n", var);
}
