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

t_expanded_list	*build_expanded_list(t_data *data, char **expanded_args,
										t_cmd_list *lst)
{
	t_expanded_list	*ret;

	ret = malloc(sizeof(t_expanded_list));
	if (!ret)
	{
		ft_free(&expanded_args);
		return (NULL);
	}
	ret->args = expanded_args;
	ret->expanded_redirs = dup_redirs(data, lst->redirs);
	if (lst->redirs && !ret->expanded_redirs)
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
		expanded_list = build_expanded_list(data, expanded_args, lst);
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
