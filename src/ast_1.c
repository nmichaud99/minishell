/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:29:17 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/23 16:29:19 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_node(t_token **tokens)
{
	t_token	*current;
	t_ast	**res;

	current = *tokens;
	while (current && current->type != PIPE)
	{
		create_leaf_node(current);
		current = current->next;
	}
	if (current->type == PIPE)
		create_pipe_node(&current);
}

t_ast	*create_pipe_node(t_token **tokens)
{
	t_ast	*res;
	t_token	*current;
	t_token	**right;

	res = malloc(sizeof(t_ast));
	if (!res)
		return (NULL);
	res->type = PIPE;
	res->cmd = NULL;
	current = *tokens;
	while (current->next != NULL && current->type != PIPE)
		current = current->next;
	right = &current->next;
	current = NULL;
	res->left = create_node(tokens);
	res->right = create_node(right);
}

t_ast	*create_leaf_node(t_token *cmd)
{
	t_ast	*res;

	res = malloc(sizeof(t_ast));
	if (!res)
		return (NULL);
	res->type = CMD;
	res->cmd = cmd;
	while (res->cmd->next != NULL && res->cmd->type != PIPE && res->cmd->type)
		cmd = cmd->next;
	res->cmd->next = NULL;
	res->left = NULL;
	res->right = NULL;
	return (res);
}
