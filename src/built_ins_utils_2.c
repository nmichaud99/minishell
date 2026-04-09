/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:00:21 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 16:00:23 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_handler(t_data *data, t_expanded_list *list)
{
	int	in;
	int	out;

	in = redir_in_handler(data, list);
	if (in == -1)
		return (0);
	else if (in != STDIN_FILENO)
	{
		if (dup2(in, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error");
		close(in);
	}
	out = redir_out_handler(list);
	if (out == -1)
		return (0);
	else if (out != STDOUT_FILENO)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(out);
	}
	return (1);
}

int	exec_built_in(t_data *data, t_expanded_list *list, int flag)
{
	int	res;
	int	saved_stdin;
	int	saved_stdout;

	if (!flag)
	{
		save_std_fds(data, &saved_stdin, &saved_stdout);
		if (!redir_handler(data, list))
		{
			if (dup2(saved_stdin, STDIN_FILENO) == -1)
				error_sys(data, "dup2 error");
			if (dup2(saved_stdout, STDOUT_FILENO) == -1)
				error_sys(data, "dup2 error");
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
	}
	res = exec_cmd(data, list);
	if (!flag)
		restore_fds(saved_stdin, saved_stdout, data);
	return (res);
}
