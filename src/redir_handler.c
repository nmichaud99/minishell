/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:14:54 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/04 16:15:05 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc(t_data *data, t_expanded_redirs *tmp)
{
	int		pipefd[2];
	char	*line;
	char	*expanded;
	int		fd;

	if (pipe(pipefd) == -1)
		error_sys(data, "pipe failure");
	while (1)
	{
		expanded = NULL;
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, tmp->file_name) == 0)
		{
			free(line);
			break ;
		}
		if (tmp->to_expand == 1)
		{
			expanded = expand_line(data, line);
			if (!expanded)
			{
				free(line);
				break ;
			}
			write(pipefd[1], expanded, ft_strlen(expanded));
		}
		else
			write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
		free(expanded);
	}
	close(pipefd[1]);
	fd = pipefd[0];
	return (fd);
}

static int	check_redirs_in(t_data *data, t_expanded_redirs *tmp, int flag)
{
	int	fd;

	fd = -1;
	if (tmp->type == REDIR_IN)
	{
		fd = open(tmp->file_name, O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				f_printf(tmp->file_name, "Permission denied\n");
			else if (errno == EISDIR)
				f_printf(tmp->file_name, "Is a directory\n");
			else
				f_printf(tmp->file_name, "No such file or directory\n");
			return (fd);
		}
		if (flag == 0)
			close(fd);
		return (fd);
	}
	else if (tmp->type == REDIR_HEREDOC)
	{
		fd = heredoc(data, tmp);
		if (flag == 0)
			close(fd);
	}
	return (fd);
}

int	redir_in_handler(t_data *data, t_expanded_list *list)
{
	t_expanded_redirs	*tmp;
	t_expanded_redirs	*last;
	int			fd;

	last = NULL;
	tmp = list->expanded_redirs;
	while (tmp)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_HEREDOC)
			last = tmp;
		tmp = tmp->next;
	}
	if (!last)
		return (STDIN_FILENO);
	tmp = list->expanded_redirs;
	while (tmp != last)
	{
		fd = check_redirs_in(data, tmp, 0);
		if (fd == -1 && (tmp->type == REDIR_IN || tmp->type == HEREDOC))
			return (fd);
		tmp = tmp->next;
	}
	fd = check_redirs_in(data, last, 1);
	return (fd);
}

static int	check_redirs_out(t_expanded_redirs *tmp, int flag)
{
	int	fd;

	fd = -1;
	if (tmp->type == REDIR_OUT)
	{
		fd = open(tmp->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			if (errno == EACCES)
				f_printf(tmp->file_name, "Permission denied\n");
			else if (errno == EISDIR)
				f_printf(tmp->file_name, "Is a directory\n");
			else
				f_printf(tmp->file_name, "No such file or directory\n");
			return (fd);
		}
		if (flag == 0)
			close(fd);
	}
	else if (tmp->type == REDIR_APPEND)
	{
		fd = open(tmp->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (f_printf(tmp->file_name, "Permission denied\n"), fd);
		if (flag == 0)
			close(fd);
	}
	return (fd);
}

int	redir_out_handler(t_expanded_list *list)
{
	t_expanded_redirs	*tmp;
	t_expanded_redirs	*last;
	int			fd;

	last = NULL;
	tmp = list->expanded_redirs;
	while (tmp)
	{
		if (tmp->type == REDIR_OUT || tmp->type == REDIR_APPEND)
			last = tmp;
		tmp = tmp->next;
	}
	if (!last)
		return (STDOUT_FILENO);
	tmp = list->expanded_redirs;
	while (tmp != last)
	{
		fd = check_redirs_out(tmp, 0);
		if (fd == -1 && (tmp->type == REDIR_OUT || tmp->type == REDIR_APPEND))
			return (fd);
		tmp = tmp->next;
	}
	fd = check_redirs_out(last, 1);
	return (fd);
}
