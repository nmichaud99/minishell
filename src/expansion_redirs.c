/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:38:23 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 15:38:24 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_redir_word(t_data *data, t_word *arg, int *i,
								t_quote_type quote)
{
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	if (quote == SPECIAL || (arg->txt[start] && arg->quoting[start] != quote))
		return (build_one_char_variable(data, i, &res, 0), res);
	if (arg->txt[start] && arg->txt[start] == '?')
		return (build_one_char_variable(data, i, &res, 1), res);
	if (arg->txt[start] && (arg->txt[start] <= '9' && arg->txt[start] >= '0'))
		return (build_one_char_variable(data, i, &res, 2), res);
	if (!(arg->txt[start] && (type_of_char(arg->txt[start],
					arg->quoting[start], quote) == 1)))
		return (build_one_char_variable(data, i, &res, 3), res);
	count = 0;
	while (arg->txt[start + count] && (type_of_char(arg->txt[start + count],
				arg->quoting[start + count], quote) >= 1))
		count++;
	res = build_variable(data, arg, start, count);
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

int	fill_node(t_data *data, t_expanded_redirs	*node, t_redirs *src)
{
	if (src->type == REDIR_HEREDOC)
		node->file_name = ft_strdup(src->filename->txt);
	else
		node->file_name = expand_redir(data, src->filename);
	if (!node->file_name)
		return (0);
	node->heredoc_name = NULL;
	node->type = src->type;
	node->to_expand = src->to_expand;
	node->next = NULL;
	return (1);
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
		if (!fill_node(data, node, src))
			return (free(node), free_expanded_redirs(&head), NULL);
		if (!head)
			head = node;
		else
			prev->next = node;
		prev = node;
		src = src->next;
	}
	return (head);
}
