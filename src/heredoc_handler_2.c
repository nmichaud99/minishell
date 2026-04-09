/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:04:05 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 16:04:19 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
