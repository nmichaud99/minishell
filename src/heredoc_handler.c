/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:55:12 by nmichaud          #+#    #+#             */
/*   Updated: 2026/04/01 19:55:14 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	g_SignalStatus = SIGINT;
	close(STDIN_FILENO);
}

int	expand_heredoc(t_data *data, char **line, char **expanded,
					t_expanded_redirs *tmp)
{
	int	fd;

	fd = data->heredoc_fd;
	if (tmp->to_expand == 1)
	{
		*expanded = expand_line(data, *line);
		if (!*expanded)
		{
			free(*line);
			return (0);
		}
		write(fd, *expanded, ft_strlen(*expanded));
	}
	else
		write(fd, *line, ft_strlen(*line));
	return (1);
}

static void	heredoc(t_data *data, t_expanded_redirs *tmp, char *filename)
{
	char	*line;
	char	*expanded;

	signal(SIGINT, sigint_handler_heredoc);
	data->heredoc_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (data->heredoc_fd == -1)
		return ;
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
		if (!expand_heredoc(data, &line, &expanded, tmp))
			break ;
		write(data->heredoc_fd, "\n", 1);
		free(line);
		free(expanded);
	}
	close(data->heredoc_fd);
}

char	*create_filename(int i)
{
	char	*number;
	char	*res;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	res = ft_strjoin("/tmp/heredoc_", number);
	free(number);
	return (res);
}

int	create_filenames(t_data *data)
{
	t_expanded_list		*tmp_cmds;
	t_expanded_redirs	*tmp_redirs;
	char				*filename;
	int					i;

	i = 0;
	tmp_cmds = data->expanded_list;
	while (tmp_cmds)
	{
		tmp_redirs = tmp_cmds->expanded_redirs;
		while (tmp_redirs)
		{
			if (tmp_redirs->type == REDIR_HEREDOC)
			{
				filename = create_filename(i);
				if (!filename)
					return (0);
				tmp_redirs->heredoc_name = filename;
				i++;
			}
			tmp_redirs = tmp_redirs->next;
		}
		tmp_cmds = tmp_cmds->next;
	}
	return (1);
}

void	heredoc_handler_utils(t_data *data)
{
	t_expanded_list		*tmp_cmds;
	t_expanded_redirs	*tmp_redirs;

	tmp_cmds = data->expanded_list;
	while (tmp_cmds)
	{
		tmp_redirs = tmp_cmds->expanded_redirs;
		while (tmp_redirs)
		{
			if (tmp_redirs->type == REDIR_HEREDOC)
				heredoc(data, tmp_redirs, tmp_redirs->heredoc_name);
			tmp_redirs = tmp_redirs->next;
		}
		tmp_cmds = tmp_cmds->next;
	}
}

int	heredoc_handler(t_data *data)
{
	int	status;

	if (!create_filenames(data))
		return (0);
	data->pid = fork();
	if (data->pid == -1)
		error_sys(data, "fork");
	if (data->pid == 0)
	{
		signal(SIGPIPE, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		heredoc_handler_utils(data);
		exit(1);
	}
	wait(&status);
	return (1);
}
