/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:46:25 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/04 15:46:37 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cmd1(t_data *data, t_expanded_list *list)
{
	char	**paths;
	char	*path_val;
	int		in;
	int		out;

	path_val = get_variable_value(data, "PATH");
	if (!path_val)
		exit_free(data, EXIT_FAILURE);
	paths = ft_split(path_val, ':');
	free(path_val);
	if (!paths)
		exit_free(data, EXIT_FAILURE);
	in = redir_in_handler(data, list);
	if (in != 0)
	{
		if (in == -1)
			return (-2);
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
	else if (list->next)
	{
		if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
	}
	if (list->next)
	{
		close(data->pipefd[0]);
		close(data->pipefd[1]);
	}
	if (*list->args)
	{
		data->full_path = find_cmd(list->args[0], paths);
		ft_free(&paths);
		if (data->full_path == NULL)
			return (1);
		if (execve(data->full_path, list->args, data->env_tab) == -1)
			error_sys(data, "exec failure");
	}
	ft_free(&paths);
	exit(0);
	return (0);
}

int	exec_cmdn(t_data *data, t_expanded_list *list, int prev_fd)
{
	char	**paths;
	char	*path_val;
	int		in;
	int		out;

	path_val = get_variable_value(data, "PATH");
	if (!path_val)
		exit_free(data, EXIT_FAILURE);
	paths = ft_split(path_val, ':');
	free(path_val);
	if (!paths)
		exit_free(data, EXIT_FAILURE);
	in = redir_in_handler(data, list);
	if (in != 0)
	{
		if (in == -1)
			return (-1);
		if (dup2(in, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error");
		close(in);
	}
	else
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error 2");
	}
	close(prev_fd);
	out = redir_out_handler(data, list);
	if (out != 0)
	{
		if (out == -1)
			return (-2);
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
	if (*list->args)
	{
		data->full_path = find_cmd(list->args[0], paths);
		ft_free(&paths);
		if (data->full_path == NULL)
			return (1);
		if (execve(data->full_path, list->args, data->env_tab) == -1)
			error_sys(data, "exec failure");
	}
	ft_free(&paths);
	exit(0);
	return (0);
}

int	exec_last_cmd(t_data *data, t_expanded_list *list, int prev_fd)
{
	char	**paths;
	char	*path_val;
	int		in;
	int		out;

	path_val = get_variable_value(data, "PATH");
	if (!path_val)
		exit_free(data, EXIT_FAILURE);
	paths = ft_split(path_val, ':');
	free(path_val);
	if (!paths)
		exit_free(data, EXIT_FAILURE);
	in = redir_in_handler(data, list);
	if (in != 0)
	{
		if (in == -1)
			return (-1);
		if (dup2(in, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error");
		close(in);
	}
	else
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error 2");
	}
	close(prev_fd);
	out = redir_out_handler(data, list);
	if (out != 0)
	{
		if (out == -1)
			return (-2);
		if (dup2(out, STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(out);
	}
	if (*list->args)
	{
		data->full_path = find_cmd(list->args[0], paths);
		ft_free(&paths);
		if (!data->full_path)
		{
			fprintf(stderr, "%s: command not found\n", list->args[0]);
			exit(127);
		}
		struct stat st;
		if (stat(data->full_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			fprintf(stderr, "%s: Is a directory\n", data->full_path);
			exit(126);
		}
		if (execve(data->full_path, list->args, data->env_tab) == -1)
		{
			if (errno == ENOENT)
				fprintf(stderr, "%s: No such file or directory\n", list->args[0]);
			else if (errno == EACCES)
				fprintf(stderr, "%s: Permission denied\n", list->args[0]);
			else if (errno == EISDIR)
				fprintf(stderr, "%s: Is a directory\n", list->args[0]);
			else if (errno == ENOEXEC)
				fprintf(stderr, "%s: cannot execute binary file\n", list->args[0]);
			else
				fprintf(stderr, "%s: Unknown execve error\n", list->args[0]);
			exit(errno == ENOENT ? 127 : 126);
		}
	}
	ft_free(&paths);
	exit(0);
	return (0);
}
