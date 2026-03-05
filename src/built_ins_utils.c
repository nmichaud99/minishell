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
		return (0); //return (exec_pwd(data, list->args));
	else if (is_built_in(*list->args) == EXPORT)
		return (exec_export(data, list->args));
	else if (is_built_in(*list->args) == UNSET)
		return (exec_unset(data, list->args));
	else if (is_built_in(*list->args) == ENV)
		return (exec_env(data));
	else if (is_built_in(*list->args) == EXIT)
		return (0); //return (exec_exit(data, list->args));
	return (0);
}

int	exec_built_in(t_data *data, t_expanded_list *list, int flag)
{
	int	in;
	int	out;
	int	res;
	int	saved_stdin;
	int	saved_stdout;

	if (!flag)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		in = redir_in_handler(data, list);
		if (in != 0)
		{
			if (in == -1)
				return (-1);
			if (dup2(in, STDIN_FILENO) == -1)
				error_sys(data, "dup2 error");
			close(in);
		}
		out = redir_out_handler(data, list);
		if (out != 0)
		{
			if (out == -1)
				return (-2);
			if (dup2(out, STDOUT_FILENO) == -1)
				error_sys(data, "dup2 error 2");
			close(out);
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
