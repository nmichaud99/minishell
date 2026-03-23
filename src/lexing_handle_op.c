/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handle_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:36:24 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 11:36:28 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_token **head, int *i)
{
	t_token	*tmp;

	tmp = new_token(PIPE, NULL);
	if (!tmp)
		return (0);
	add_token(head, tmp);
	(*i)++;
	return (1);
}

int	handle_indir(t_token **head, char *str, int *i)
{
	t_token	*tmp;

	if (str[*i + 1] && str[*i + 1] == '<')
	{
		tmp = new_token(HEREDOC, NULL);
		if (!tmp)
			return (0);
		add_token(head, tmp);
		(*i) += 2;
	}
	else
	{
		tmp = new_token(IN_DIR, NULL);
		if (!tmp)
			return (0);
		add_token(head, tmp);
		(*i)++;
	}
	return (1);
}

int	handle_outdir(t_token **head, char *str, int *i)
{
	t_token	*tmp;

	if (str[*i + 1] && str[*i + 1] == '>')
	{
		tmp = new_token(APPEND, NULL);
		if (!tmp)
			return (0);
		add_token(head, tmp);
		(*i) += 2;
	}
	else
	{
		tmp = new_token(OUT_DIR, NULL);
		if (!tmp)
			return (0);
		add_token(head, tmp);
		(*i)++;
	}
	return (1);
}

int	handle_operators(t_token **head, char *str, int *i)
{
	if (str[*i] == '|')
	{
		if (!handle_pipe(head, i))
			return (0);
	}
	else if (str[*i] == '<')
	{
		if (!handle_indir(head, str, i))
			return (0);
	}
	else if (str[*i] == '>')
	{
		if (!handle_outdir(head, str, i))
			return (0);
	}
	return (1);
}
