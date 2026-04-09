/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:08:06 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/16 16:08:07 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signalstatus = 0;

static void	wait_and_return(t_data *data)
{
	int	status;
	int	i;

	data->last_status = 0;
	i = 1;
	while (i > 0)
	{
		i = wait(&status);
		if (i == data->last_pid)
			data->last_status = status;
	}
	if (WIFEXITED(data->last_status))
		data->exit_status = (WEXITSTATUS(data->last_status));
	else if (WIFSIGNALED(data->last_status))
		data->exit_status = 128 + WTERMSIG(data->last_status);
}

int	treat_cmd_line(t_data *data, t_expanded_list **list)
{
	if (!lexing(data))
		return (0);
	if (!syntax_check(data))
	{
		data->exit_status = 2;
		return (0);
	}
	if (!parsing(data))
		return (0);
	if (!expansion(data))
		return (0);
	if (!heredoc_handler(data))
		return (0);
	data->env_tab = get_env_tab(data);
	if (!data->env_tab)
		return (0);
	*list = data->expanded_list;
	if (!data->expanded_list)
		return (0);
	return (1);
}

int	ft_readline(t_data *data)
{
	data->line = readline("minishell$ ");
	if (!data->line)
	{
		printf("exit\n");
		return (0);
	}
	if (*(data->line))
		add_history(data->line);
	return (1);
}

void	ft_execution(t_data *data, t_expanded_list *list)
{
	int	prev_fd;

	prev_fd = -1;
	if (list->next || !*(list->args) || is_built_in(*list->args) == NO)
	{
		while (list)
		{
			data->last_pid = pipe_creator(data, &prev_fd, list);
			list = list->next;
		}
		wait_and_return(data);
	}
	else
		data->exit_status = exec_built_in(data, list, 0);
}

int	main(int ac, char **av, char **env)
{
	t_data			data;
	t_expanded_list	*list;

	if (!check_args_and_int(ac, av, env, &data))
		return (1);
	while (1)
	{
		init_data(&data, env, 1);
		set_signals_ignore();
		set_signals_interactive();
		if (!ft_readline(&data))
			break ;
		if (g_signalstatus == SIGINT)
			data.exit_status = 130;
		g_signalstatus = 0;
		list = NULL;
		if (!treat_cmd_line(&data, &list))
			continue ;
		set_signals_exec();
		ft_execution(&data, list);
		set_signals_interactive();
	}
	rl_clear_history();
	exit_free(&data, 0);
}
