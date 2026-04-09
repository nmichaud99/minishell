/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:55:41 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 15:55:42 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**dup_args(char **str)
{
	int		i;
	char	**res;

	i = 0;
	while (str[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
	{
		res[i] = ft_strdup(str[i]);
		if (!res[i])
			return (ft_free(&res), NULL);
		i++;
	}
	res[i] = NULL;
	return (res);
}

int	add_arg_node(t_arg_list **head, char **str)
{
	t_arg_list	*tmp;
	t_arg_list	*new;
	char		**args;

	new = malloc(sizeof(t_arg_list));
	if (!new)
		return (0);
	args = dup_args(str);
	if (!args)
		return (free(new), 0);
	new->args = args;
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return (1);
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}

int	count_expanded_args(t_arg_list **head)
{
	int			i;
	int			count;
	t_arg_list	*tmp;

	count = 0;
	tmp = *head;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
			i++;
		count += i;
		tmp = tmp->next;
	}
	return (count);
}

char	**build_expanded_args(t_arg_list **head)
{
	char		**res;
	int			i;
	int			j;
	t_arg_list	*tmp;

	j = count_expanded_args(head);
	res = malloc(sizeof(char *) * (j + 1));
	if (!res)
		return (NULL);
	j = 0;
	tmp = *head;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			res[j] = ft_strdup(tmp->args[i++]);
			if (!res[j])
				return (ft_free(&res), NULL);
			j++;
		}
		tmp = tmp->next;
	}
	res[j] = NULL;
	return (res);
}

char	**get_expanded_args(t_data *data, t_cmd_list *lst)
{
	t_arg_list	*head;
	char		**tmp;
	char		**ret;
	int			i;

	head = NULL;
	i = 0;
	while (lst->args[i])
	{
		tmp = expand_arg(data, lst->args[i]);
		if (!tmp)
			return (free_arg_list(&head), NULL);
		if (!add_arg_node(&head, tmp))
			return (free_arg_list(&head), ft_free(&tmp), NULL);
		ft_free(&tmp);
		i++;
	}
	ret = build_expanded_args(&head);
	free_arg_list(&head);
	return (ret);
}
