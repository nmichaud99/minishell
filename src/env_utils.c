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

char	**malloc_env_tab(t_data *data)
{
	t_env	*tmp;
	int		i;
	char	**res;

	tmp = data->env;
	i = 0;
	while (tmp)
	{
		if (tmp->has_value)
			i++;
		tmp = tmp->next;
	}
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	return (res);
}

int	add_env_var(char **res, int *i, t_env *env_var)
{
	char	*tmp_str;

	tmp_str = ft_strjoin(env_var->key, "=");
	if (!tmp_str)
		return (0);
	res[*i] = ft_strjoin(tmp_str, env_var->value);
	free(tmp_str);
	if (!res[*i])
		return (0);
	(*i)++;
	return (1);
}

char	**get_env_tab(t_data *data)
{
	t_env	*tmp;
	char	**res;
	int		i;

	res = malloc_env_tab(data);
	if (!res)
		return (NULL);
	i = 0;
	tmp = data->env;
	while (tmp)
	{
		if (tmp->has_value)
		{
			if (!add_env_var(res, &i, tmp))
			{
				ft_free(&res);
				return (NULL);
			}
		}
		tmp = tmp->next;
	}
	res[i] = NULL;
	return (res);
}

int	build_minimal_env(t_data *data)
{
	char	*pwd;
	char	*line;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (0);
	line = ft_strjoin("PWD=", pwd);
	free(pwd);
	if (!line)
		exit_free(data, EXIT_FAILURE);
	add_env_node(data, line, 1);
	free(line);
	return (1);
}
