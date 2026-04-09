/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:11:53 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 13:11:55 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arg_list(t_arg_list **head)
{
	t_arg_list	*current;
	t_arg_list	*tmp;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		ft_free(&current->args);
		tmp = current;
		current = current->next;
		free(tmp);
	}
	*head = NULL;
}

void	free_data(t_data *data)
{
	free_token(&data->tokens);
	free_list(&data->cmd_list);
	free(data->full_path);
	data->full_path = NULL;
	free(data->line);
	data->line = NULL;
}

void	exit_free(t_data *data, int status)
{
	if (!data)
		exit(status);
	free_data(data);
	free_expanded_list(&data->expanded_list);
	free_env(&data->env);
	ft_free(&data->env_tab);
	exit(status);
}

void	free_all(t_data *data)
{
	if (!data)
		exit(data->exit_status);
	free_data(data);
	free_expanded_list(&data->expanded_list);
	free_env(&data->env);
	ft_free(&data->env_tab);
}

void	error_sys(t_data *data, char *s)
{
	perror(s);
	exit_free(data, data->exit_status);
}
