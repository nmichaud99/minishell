/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:00:16 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 16:00:18 by fjerrige         ###   ########.fr       */
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

void	restore_fds(int saved_stdin, int saved_stdout, t_data *data)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		error_sys(data, "dup2 error");
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		error_sys(data, "dup2 error");
	close(saved_stdin);
	close(saved_stdout);
}
