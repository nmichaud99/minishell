/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:06:56 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 13:07:06 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd1(t_data *data, t_expanded_list *list)
{
	int		out;
	int		in;

	rl_clear_history();
	in = redir_in(data, list, -1, 1);
	out = redir_out_handler(list);
	if (out == -1)
	{
		if (list->next)
		{
			close(data->pipefd[0]);
			close(data->pipefd[1]);
		}
		exit_free(data, 1);
	}
	if (out != STDOUT_FILENO)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(out);
	}
	else if (list->next)
	{
		if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(data->pipefd[0]);
		close(data->pipefd[1]);
	}
	get_path_and_exec(data, list);
	exit_free(data, 0);
}

void	exec_cmdn(t_data *data, t_expanded_list *list, int prev_fd)
{
	int		out;
	int		in;

	rl_clear_history();
	in = redir_in(data, list, prev_fd, 2);
	out = redir_out_handler(list);
	if (out == -1)
	{
		close(prev_fd);
		close(data->pipefd[0]);
		close(data->pipefd[1]);
		exit_free(data, 1);
	}
	if (out != STDOUT_FILENO)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(out);
	}
	else
	{
		if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
	}
	close(data->pipefd[0]);
	close(data->pipefd[1]);
	get_path_and_exec(data, list);
	exit_free(data, 0);
}

void	exec_last_cmd(t_data *data, t_expanded_list *list, int prev_fd)
{
	int		out;
	int		in;

	rl_clear_history();
	in = redir_in(data, list, prev_fd, 3);
	out = redir_out_handler(list);
	if (out == -1)
	{
		close(prev_fd);
		exit_free(data, 1);
	}
	if (out != STDOUT_FILENO)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(out);
	}
	get_path_and_exec(data, list);
	exit_free(data, 0);
}
