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

char	*expand_redir_word(t_data *data, t_word *arg, int *i, t_quote_type quote)
{
	char	*tmp;
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	if (quote == SPECIAL)
	{
		(*i) = start;
		res = ft_strdup("");
		if (!res)
			return (NULL);
		return (res);
	}
	if (quote == SPECIAL && !arg->txt[start])
	{
		(*i) = start + 1;
		res = ft_strdup("$");
		if (!res)
			return (NULL);
		return (res);
	}
	if (arg->txt[start] && arg->quoting[start] != quote && quote != SPECIAL)
	{
		(*i) = start;
		res = ft_strdup("");
		if (!res)
			return (NULL);
		return (res);
	}
	if (arg->txt[start] == '?')
	{
		(*i) = start + 1;
		tmp = "?";
		res = get_variable_value(data, tmp);
		if (!res)
			return (NULL);
		return (res);
	}
	if (arg->txt[start] && (arg->txt[start] <= '9' && arg->txt[start] >= '0'))
	{
		(*i) = start + 1;
		res = ft_strdup("");
		if (!res)
			return (NULL);
		return (res);
	}
	if (!(arg->txt[start] && (type_of_char(arg->txt[start],
			arg->quoting[start], quote) == 1)))
	{
		(*i)++;
		res = ft_strdup("$");
		if (!res)
			return (NULL);
		return (res);
	}
	count = 0;
	while (arg->txt[start + count] && (type_of_char(arg->txt[start + count],
			arg->quoting[start + count], quote) >= 1))
		count++;
	tmp = malloc(count + 1);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, &arg->txt[start], count);
	tmp[count] = 0;
	res = get_variable_value(data, tmp);
	free(tmp);
	if (!res)
		return (NULL);
	*i = start + count;
	return (res);
}

char	*expand_redir(t_data *data, t_word *arg)
{
	char			*res;
	char			*variable;
	int				i;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (arg->txt[i])
	{
		if (arg->txt[i] == '$' && arg->quoting[i] != SINGLE)
		{
			variable = expand_redir_word(data, arg, &i, arg->quoting[i]);
			if (!variable || !append_variable(&res, &variable))
				return (free(res), NULL);
		}
		else
		{
			if (!append_char(&res, arg->txt[i]))
				return (free(res), NULL);
			i++;
		}
	}
	return (res);
}

t_expanded_redirs	*dup_redirs(t_data *data, t_redirs *src)
{
	t_expanded_redirs	*head;
	t_expanded_redirs	*prev;
	t_expanded_redirs	*node;

	head = NULL;
	prev = NULL;
	while (src)
	{
		node = malloc(sizeof(t_expanded_redirs));
		if (!node)
			return (free_expanded_redirs(&head), NULL);
		if (src->type == REDIR_HEREDOC)
			node->file_name = ft_strdup(src->filename->txt);
		else
			node->file_name = expand_redir(data, src->filename);
		if (!node->file_name)
			return (free(node), free_expanded_redirs(&head), NULL);
		node->heredoc_name = NULL;
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

t_expanded_list	*build_expanded_list(t_data *data, char **expanded_args, t_cmd_list *lst)
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
