/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handle_words.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:39:19 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 11:39:21 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_squote(char *str, int *i, char **txt, t_quote_type **quoting)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		if (!lex_append_char(txt, quoting, str[(*i)++], SINGLE))
		{
			free(*txt);
			free(*quoting);
			return (0);
		}
	}
	if (str[*i] != '\'')
	{
		free(*txt);
		free(*quoting);
		return (-1);
	}
	(*i)++;
	return (1);
}

int	handle_dquote(char *str, int *i, char **txt, t_quote_type **quoting)
{
	(*i)++;
	while (str[*i] && str[*i] != '"')
	{
		if (!lex_append_char(txt, quoting, str[(*i)++], DOUBLE))
		{
			free(*txt);
			free(*quoting);
			return (0);
		}
	}
	if (str[*i] != '"')
	{
		free(*txt);
		free(*quoting);
		return (-1);
	}
	(*i)++;
	return (1);
}

int	handle_char(char *str, int *i, char **txt, t_quote_type **quoting)
{
	if (!lex_append_char(txt, quoting, str[(*i)++], NONE))
	{
		free(*txt);
		free(*quoting);
		return (0);
	}
	return (1);
}

int	build_word(char *str, int *i, char **txt, t_quote_type **quoting)
{
	int	ret;

	while (str[*i] && !is_space(str[*i]) && !is_operator(str[*i]))
	{
		if (str[*i] == '\'')
		{
			ret = handle_squote(str, i, txt, quoting);
			if (ret != 1)
				return (ret);
		}
		else if (str[*i] == '"')
		{
			ret = handle_dquote(str, i, txt, quoting);
			if (ret != 1)
				return (ret);
		}
		else
		{
			if (!handle_char(str, i, txt, quoting))
				return (0);
		}
	}
	return (1);
}

int	handle_word(t_token **head, char *str, int *i)
{
	int				ret;
	t_word			*word;
	t_token			*tmp;
	char			*txt;
	t_quote_type	*quoting;

	txt = NULL;
	quoting = NULL;
	ret = build_word(str, i, &txt, &quoting);
	if (ret != 1)
		return (ret);
	word = malloc(sizeof(t_word));
	if (!word)
	{
		free(txt);
		free(quoting);
		return (0);
	}
	word->txt = txt;
	word->quoting = quoting;
	tmp = new_token(WORD, word);
	if (!tmp)
		return (free_word(&word), 0);
	add_token(head, tmp);
	return (1);
}
