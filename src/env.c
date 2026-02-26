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
	(void)env;
	int		i;
	t_env	*tmp;

	i = 0;
	while (env[i])
	{
		//printf("** %s\n", env[i]);
		add_env_node(data, env[i]);
		i++;
	}
	tmp = data->env;
	while (tmp && tmp->next)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	printf("%s=%s\n", tmp->key, tmp->value);
}

// Add Variable to environment
/*
	- If nom de variable deja existant :
		- if new value = old value -> return
		- else modifier la valeur

	- Else if nom de variable existe pas, la creer
*/

void	add_or_modify_env_node(t_data *data, char *new_var)
{
	t_env	*tmp;
	char	*new_key;
	char	*new_value;

	new_key = get_variable_key(new_var);
	new_value = ft_strdup(ft_strchr(new_var, '=') + 1);
	tmp = data->env;
	while (tmp && tmp->next)
	{
		if (tmp->key == new_key)
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

// Modify Variable
