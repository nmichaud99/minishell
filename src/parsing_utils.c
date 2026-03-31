/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:49:51 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 11:49:54 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(t_token_type type)
{
	if (type == IN_DIR || type == OUT_DIR || type == HEREDOC || type == APPEND)
		return (1);
	return (0);
}

t_redir_type	convert_types(t_token_type token_type)
{
	t_redir_type	type;

	if (token_type == IN_DIR)
		type = REDIR_IN;
	if (token_type == OUT_DIR)
		type = REDIR_OUT;
	if (token_type == HEREDOC)
		type = REDIR_HEREDOC;
	if (token_type == APPEND)
		type = REDIR_APPEND;
	return (type);
}

void	null_init(t_word **args, int nb_args)
{
	int	i;

	i = 0;
	while (i < nb_args)
		args[i++] = NULL;
}

void	fill_redir_node(t_redirs **redir, t_word *filename,
							t_token_type type, t_quote_type *quoting)
{
	int	i;

	i = 0;
	(*redir)->to_expand = 1;
	while (filename->txt[i])
	{
		if (quoting[i] == SINGLE || quoting[i] == DOUBLE)
			(*redir)->to_expand = 0;
		i++;
	}
	if (!filename->txt[0] && (quoting[0] == SINGLE || quoting[0] == DOUBLE))
		(*redir)->to_expand = 0;
	(*redir)->filename = filename;
	(*redir)->type = convert_types(type);
	(*redir)->next = NULL;
}
