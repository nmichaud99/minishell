/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:40:16 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 16:40:17 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_token(t_token **head)
{
	t_token *tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		free((*head)->str);
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
		tmp = NULL;
	}
	head = NULL;
}

t_token	*new_token(t_token_type type, char *content)
{
	t_token	*res;

	res = malloc(sizeof(t_token));
	if (!res)
		return (NULL);
	res->type = type;
	res->str = content;
	res->next = NULL;
	return (res);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_node	*new_node(t_data *data, t_node_type type, t_token *cmd)
{
	t_node	*res;

	res = malloc(sizeof(t_node));
	if (!res)
		return (NULL);
	res->type = type;
	res->cmd = cmd;
	while (res->cmd->next != NULL && res->cmd->type != PIPE && res->cmd->type != REDIR)
		cmd = cmd->next;
	res->cmd->next = NULL
	if (type = PIPE)
	{
		res->left = data->nodes;
		res->right = data->nodes;
	}
	return (res);
}
