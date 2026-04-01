/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:06:50 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 13:06:53 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exec(t_data *data, t_expanded_list *list)
{
	if (execve(data->full_path, list->args, data->env_tab) == -1)
	{
		if (errno == ENOENT)
			f_printf(list->args[0], "No such file or directory\n");
		else if (errno == EACCES)
			f_printf(list->args[0], "Permission denied\n");
		else if (errno == EISDIR)
			f_printf(list->args[0], "Is a directory\n");
		else if (errno == ENOEXEC)
			f_printf(list->args[0], "cannot execute binary file\n");
		else
			f_printf(list->args[0], "Unknown execve error\n");
		if (errno == ENOENT)
			exit_free(data, 127);
		else
			exit_free(data, 126);
	}
}

int	redir_in(t_data *data, t_expanded_list *list, int prev_fd, int first)
{
	int	in;

	in = redir_in_handler(data, list);
	if (in == -1)
	{
		if (first > 1)
			close(prev_fd);
		if ((first == 1 && list->next) || first == 2)
		{
			close(data->pipefd[0]);
			close(data->pipefd[1]);
		}
		exit_free(data, 1);
	}
	if (in != STDIN_FILENO)
	{
		if (dup2(in, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(in);
	}
	else if (in == STDIN_FILENO && first > 1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error 2");
	}
	if (first > 1)
		close(prev_fd);
	return (in);
}

char	**get_all_paths(t_data *data)
{
	char	*path_val;
	char	**paths;

	path_val = get_variable_value(data, "PATH");
	if (!path_val)
		exit_free(data, EXIT_FAILURE);
	paths = ft_split(path_val, ':');
	free(path_val);
	if (!paths)
		exit_free(data, EXIT_FAILURE);
	return (paths);
}

void	handle_path_error(t_data *data, int c, char *m, t_expanded_list *list)
{
	if (c == 126)
		f_printf(data->full_path, m);
	else if (c == 127)
		f_printf(list->args[0], m);
	exit_free(data, c);
}

void	get_path_and_exec(t_data *data, t_expanded_list *list)
{
	int			status;
	char		**paths;
	struct stat	st;

	if (*list->args)
	{
		if (is_built_in(list->args[0]) != NO)
		{
			status = exec_built_in(data, list, 1);
			exit_free(data, status);
		}
		paths = get_all_paths(data);
		data->full_path = find_cmd(list->args[0], paths);
		ft_free(&paths);
		if (!data->full_path)
			handle_path_error(data, 127, "command not found\n", list);
		if (stat(data->full_path, &st) == 0 && S_ISDIR(st.st_mode))
			handle_path_error(data, 126, "Is a directory\n", list);
		ft_exec(data, list);
	}
}

/*void	exec_cmd1(t_data *data, t_expanded_list *list)
{
	char	**paths;
	char	*path_val;
	int		in;
	int		out;
	int		status;
	struct	stat st;

	rl_clear_history();
	in = redir_in_handler(data, list);
	if (in == -1)
		exit_free(data, 1);
	if (in != STDIN_FILENO)
	{
		if (dup2(in, STDIN_FILENO) == -1)
			error_sys(data, "dup2 error");
		close(in);
	}
	out = redir_out_handler(list);
	if (out == -1)
		exit_free(data, 1);
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
	if (*list->args)
	{
		if (is_built_in(list->args[0]) != NO)
		{
			status = exec_built_in(data, list, 1);
			exit_free(data, status);
		}
		path_val = get_variable_value(data, "PATH");
		if (!path_val)
			exit_free(data, EXIT_FAILURE);
		paths = ft_split(path_val, ':');
		free(path_val);
		if (!paths)
			exit_free(data, EXIT_FAILURE);
		data->full_path = find_cmd(list->args[0], paths);
		ft_free(&paths);
		if (!data->full_path)
		{
			f_printf(list->args[0], "command not found\n");
			exit_free(data, 127);
		}
		if (stat(data->full_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			f_printf(data->full_path, "Is a directory\n");
			exit_free(data, 126);
		}
		if (execve(data->full_path, list->args, data->env_tab) == -1)
		{
			if (errno == ENOENT)
				f_printf(list->args[0], "No such file or directory\n");
			else if (errno == EACCES)
				f_printf(list->args[0], "Permission denied\n");
			else if (errno == EISDIR)
				f_printf(list->args[0], "Is a directory\n");
			else if (errno == ENOEXEC)
				f_printf(list->args[0], "cannot execute binary file\n");
			else
				f_printf(list->args[0], "Unknown execve error\n");
			exit_free(data, errno == ENOENT ? 127 : 126);
		}
	}
	exit_free(data, 0);
}

void	exec_cmdn(t_data *data, t_expanded_list *list, int prev_fd)
{
	char	**paths;
	char	*path_val;
	int		in;
	int		out;
	int		status;
	struct	stat st;

	rl_clear_history();
	in = redir_in_handler(data, list);
	if (in == -1)
		exit_free(data, 1);
	if (in != STDIN_FILENO)
	{
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
	out = redir_out_handler(list);
	if (out == -1)
		exit_free(data, 1);
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
	if (*list->args)
	{
		if (is_built_in(list->args[0]) != NO)
		{
			status = exec_built_in(data, list, 1);
			exit_free(data, status);
		}
		path_val = get_variable_value(data, "PATH");
		if (!path_val)
			exit_free(data, EXIT_FAILURE);
		paths = ft_split(path_val, ':');
		free(path_val);
		if (!paths)
			exit_free(data, EXIT_FAILURE);
		data->full_path = find_cmd(list->args[0], paths);
		ft_free(&paths);
		if (!data->full_path)
		{
			f_printf(list->args[0], "command not found\n");
			exit_free(data, 127);
		}
		if (stat(data->full_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			f_printf(data->full_path, "Is a directory\n");
			exit_free(data, 126);
		}
		if (execve(data->full_path, list->args, data->env_tab) == -1)
		{
			if (errno == ENOENT)
				f_printf(list->args[0], "No such file or directory\n");
			else if (errno == EACCES)
				f_printf(list->args[0], "Permission denied\n");
			else if (errno == EISDIR)
				f_printf(list->args[0], "Is a directory\n");
			else if (errno == ENOEXEC)
				f_printf(list->args[0], "cannot execute binary file\n");
			else
				f_printf(list->args[0], "Unknown execve error\n");
			exit_free(data, errno == ENOENT ? 127 : 126);
		}
	}
	exit_free(data, 0);
}

void	exec_last_cmd(t_data *data, t_expanded_list *list, int prev_fd)
{
	char	**paths;
	char	*path_val;
	int		in;
	int		out;
	int		status;
	struct	stat st;

	rl_clear_history();
	in = redir_in_handler(data, list);
	if (in == -1)
		exit_free(data, 1);
	if (in != STDIN_FILENO)
	{
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
	out = redir_out_handler(list);
	if (out == -1)
		exit_free(data, 1);
	if (out != STDOUT_FILENO)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
			error_sys(data, "dup2 error 2");
		close(out);
	}
	if (*list->args)
	{
		if (is_built_in(list->args[0]) != NO)
		{
			status = exec_built_in(data, list, 1);
			exit_free(data, status);
		}
		path_val = get_variable_value(data, "PATH");
		if (!path_val)
			exit_free(data, EXIT_FAILURE);
		paths = ft_split(path_val, ':');
		free(path_val);
		if (!paths)
			exit_free(data, EXIT_FAILURE);
		data->full_path = find_cmd(list->args[0], paths);
		ft_free(&paths);
		if (!data->full_path)
		{
			f_printf(list->args[0], "command not found\n");
			exit_free(data, 127);
		}
		if (stat(data->full_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			f_printf(data->full_path, "Is a directory\n");
			exit_free(data, 126);
		}
		if (execve(data->full_path, list->args, data->env_tab) == -1)
		{
			if (errno == ENOENT)
				f_printf(list->args[0], "No such file or directory\n");
			else if (errno == EACCES)
				f_printf(list->args[0], "Permission denied\n");
			else if (errno == EISDIR)
				f_printf(list->args[0], "Is a directory\n");
			else if (errno == ENOEXEC)
				f_printf(list->args[0], "cannot execute binary file\n");
			else
				f_printf(list->args[0], "Unknown execve error\n");
			exit_free(data, errno == ENOENT ? 127 : 126);
		}
	}
	exit_free(data, 0);
}*/
