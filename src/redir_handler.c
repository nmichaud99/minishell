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

int	redir_in_handler(t_data *data, t_expanded_list *list)
{
	t_redirs	*tmp;
	t_redirs	*last;
	int			fd;
	int			new_fd;
	int			pipefd[2];
	char		*line;

	last = NULL;
	tmp = list->redirs;
	while (tmp)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_HEREDOC)
			last = tmp;
		tmp = tmp->next;
	}
	if (!last)
		return (STDIN_FILENO);
	tmp = list->redirs;
	while (tmp != last)
	{
		if (last->type == REDIR_IN)
		{
			new_fd = open(last->file_name, O_RDONLY);
			if (new_fd == -1)
				return (-1);
			close(new_fd);
		}
		else
		{
			if (pipe(pipefd) == -1)
				error_sys(data, "pipe failure");
			while (1)
			{
				line = readline("> ");
				if (!line)
					break ;
				if (ft_strcmp(line, last->file_name) == 0)
				{
					free(line);
					break ;
				}
				write(pipefd[1], line, ft_strlen(line));
				write(pipefd[1], "\n", 1);
				free(line);
			}
			close(pipefd[1]);
			new_fd = pipefd[0];
			close(new_fd);
		}
		tmp = tmp->next;
	}
	if (last->type == REDIR_IN)
			fd = open(last->file_name, O_RDONLY);
	else
	{
		if (pipe(pipefd) == -1)
			error_sys(data, "pipe failure");
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (ft_strcmp(line, last->file_name) == 0)
			{
				free(line);
				break ;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		fd = pipefd[0];
	}
	return (fd);
}

int	redir_out_handler(t_data *data, t_expanded_list *list)
{
	t_redirs	*tmp;
	t_redirs	*last;
	int			fd;
	int			new_fd;

	(void)data;
	last = NULL;
	tmp = list->redirs;
	while (tmp)
	{
		if (tmp->type == REDIR_OUT || tmp->type == REDIR_APPEND)
			last = tmp;
		tmp = tmp->next;
	}
	if (!last)
		return (STDOUT_FILENO);
	tmp = list->redirs;
	while (tmp != last)
	{
		if (tmp->type == REDIR_OUT)
		{
			new_fd = open(tmp->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (new_fd == -1)
				return (-1);
			else
				close(new_fd);
		}
		else if (tmp->type == REDIR_APPEND)
		{
			new_fd = open(tmp->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (new_fd == -1)
				return (-1);
			else
				close(new_fd);
		}
		tmp = tmp->next;
	}
	if (last->type == REDIR_OUT)
		fd = open(last->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(last->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
