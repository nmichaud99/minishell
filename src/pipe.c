/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 12:44:25 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/04 12:44:35 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_if(t_data *data, int *prev_fd, t_expanded_list *list)
{
	*(data->exit_status) = 0;
	if (list == data->expanded_list)
		exec_cmd1(data, list);
	else if (list->next)
		exec_cmdn(data, list, *prev_fd);
	else
		exec_last_cmd(data, list, *prev_fd);
}

int	pipe_creator(t_data *data, int *prev_fd, t_expanded_list *list)
{
	if (list->next)
	{
		if (pipe(data->pipefd) == -1)
			error_sys(data, "pipe failure");
	}
	free(data->full_path);
	data->full_path = NULL;
	data->pid = fork();
	if (data->pid == -1)
		error_sys(data, "fork");
	if (data->pid == 0)
		exec_if(data, prev_fd, list);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (list->next)
	{
		*prev_fd = data->pipefd[0];
		//close(data->pipefd[0]);
	}
	return (data->pid);
}
