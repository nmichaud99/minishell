/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:13:20 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 16:13:21 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_env_tab(char **env, t_data *data)
{
	int	i;

	if (!*env && build_minimal_env(data))
		return (1);
	i = 0;
	while (env[i])
	{
		if (!add_env_node(data, env[i], 1))
			return (0);
		i++;
	}
	return (1);
}

void	re_init(t_data *data)
{
	data->saved_stdin = -1;
	data->saved_stdout = -1;
	free_data(data);
	free_expanded_list(&data->expanded_list);
	ft_free(&data->env_tab);
	free(data->line);
	data->line = NULL;
}

int	init_data(t_data *data, char **env, int flag)
{
	if (flag == 0)
	{
		data->line = NULL;
		data->tokens = NULL;
		data->cmd_list = NULL;
		data->expanded_list = NULL;
		data->env = NULL;
		data->env_tab = NULL;
		data->full_path = NULL;
		data->exit_status = 0;
		data->saved_stdin = -1;
		data->saved_stdout = -1;
		if (!init_env_tab(env, data))
			return (0);
		return (1);
	}
	else
		re_init(data);
	return (1);
}

int	check_args_and_int(int ac, char **av, char **env, t_data *data)
{
	(void)av;
	if (ac != 1 || !isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (ft_putstr_fd("./minishell: too many arguments\n", 2), 0);
	if (!init_data(data, env, 0))
		return (0);
	return (1);
}
