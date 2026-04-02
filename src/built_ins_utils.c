/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 09:46:33 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/05 09:46:46 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin_type	is_built_in(char *arg)
{
	if (!ft_strcmp(arg, "echo"))
		return (ECHO);
	else if (!ft_strcmp(arg, "cd"))
		return (CD);
	else if (!ft_strcmp(arg, "pwd"))
		return (PWD);
	else if (!ft_strcmp(arg, "export"))
		return (EXPORT);
	else if (!ft_strcmp(arg, "unset"))
		return (UNSET);
	else if (!ft_strcmp(arg, "env"))
		return (ENV);
	else if (!ft_strcmp(arg, "exit"))
		return (EXIT);
	else
		return (NO);
}

int	exec_cmd(t_data *data, t_expanded_list *list)
{
	if (is_built_in(*list->args) == ECHO)
		return (exec_echo(list->args));
	else if (is_built_in(*list->args) == CD)
		return (exec_cd(data, list->args));
	else if (is_built_in(*list->args) == PWD)
		return (exec_pwd(data, list->args));
	else if (is_built_in(*list->args) == EXPORT)
		return (exec_export(data, list->args));
	else if (is_built_in(*list->args) == UNSET)
		return (exec_unset(data, list->args));
	else if (is_built_in(*list->args) == ENV)
		return (exec_env(data, list->args));
	else if (is_built_in(*list->args) == EXIT)
		return (exec_exit(data, list->args));
	return (0);
}

void	save_std_fds(t_data *data, int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
		error_sys(data, "dup error");
	data->saved_stdin = *saved_stdin;
	data->saved_stdout = *saved_stdout;
}

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
	{
		if (dup2(saved_stdin, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error");
		if (dup2(saved_stdout, STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error");
		close(saved_stdin);
		close(saved_stdout);
	}
	return (res);
}
