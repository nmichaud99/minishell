/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:03:19 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/02 19:03:23 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirs	*dup_redirs(t_redirs *src)
{
	t_redirs	*head;
	t_redirs	*prev;
	t_redirs	*node;

	head = NULL;
	prev = NULL;
	while (src)
	{
		node = malloc(sizeof(t_redirs));
		if (!node)
			return (free_redirs(&head), NULL);
		node->file_name = ft_strdup(src->file_name);
		if (!node->file_name)
			return (free(node), free_redirs(&head), NULL);
		node->type = src->type;
		node->to_expand = src->to_expand;
		node->next = NULL;
		if (!head)
			head = node;
		else
			prev->next = node;
		prev = node;
		src = src->next;
	}
	return (head);
}

t_expanded_list	*build_expanded_list(char **expanded_args, t_cmd_list *lst)
{
	t_expanded_list	*ret;

	ret = malloc(sizeof(t_expanded_list));
	if (!ret)
	{
		ft_free(&expanded_args);
		return (NULL);
	}
	ret->args = expanded_args;
	ret->redirs = dup_redirs(lst->redirs);
	if (lst->redirs && !ret->redirs)
	{
		ft_free(&expanded_args);
		free(ret);
		return (NULL);
	}
	ret->next = NULL;
	return (ret);
}

int	expansion(t_data *data)
{
	t_cmd_list		*lst;
	char			**expanded_args;
	t_expanded_list	*expanded_list;
	t_expanded_list	*prev;

	prev = NULL;
	lst = data->cmd_list;
	while (lst)
	{
		expanded_args = get_expanded_args(data, lst);
		if (!expanded_args)
			return (free_expanded_list(&data->expanded_list), 0);
		expanded_list = build_expanded_list(expanded_args, lst);
		if (!expanded_list)
			return (free_expanded_list(&data->expanded_list), 0);
		if (!data->expanded_list)
			data->expanded_list = expanded_list;
		else
			prev->next = expanded_list;
		prev = expanded_list;
		lst = lst->next;
	}
	return (1);
}
