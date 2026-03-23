/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:10:48 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 13:11:02 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redirs **redirs)
{
	t_redirs	*tmp;

	if (!redirs || !*redirs)
		return ;
	while (*redirs)
	{
		free((*redirs)->file_name);
		tmp = *redirs;
		*redirs = (*redirs)->next;
		free(tmp);
		tmp = NULL;
	}
	*redirs = NULL;
}

void	free_env(t_env **env)
{
	t_env	*tmp;

	if (!env || !*env)
		return ;
	while (*env)
	{
		free((*env)->key);
		free((*env)->value);
		tmp = *env;
		*env = (*env)->next;
		free(tmp);
		tmp = NULL;
	}
	*env = NULL;
}

void	free_list(t_cmd_list **list)
{
	t_cmd_list	*tmp;

	if (!list || !*list)
		return ;
	while (*list)
	{
		free_word_tab(&(*list)->args);
		free_redirs(&(*list)->redirs);
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
		tmp = NULL;
	}
	*list = NULL;
}

void	free_expanded_list(t_expanded_list **list)
{
	t_expanded_list	*tmp;

	if (!list || !*list)
		return ;
	while (*list)
	{
		ft_free(&(*list)->args);
		free_redirs(&(*list)->redirs);
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
		tmp = NULL;
	}
	*list = NULL;
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
