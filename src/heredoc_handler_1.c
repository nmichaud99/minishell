/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:03:59 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 16:04:01 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	heredoc(t_data *data, t_expanded_redirs *tmp, char *filename)
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
