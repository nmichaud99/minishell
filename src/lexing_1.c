/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:11:50 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 15:11:50 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_char(char **txt, t_quote_type **quoting,
							char c, t_quote_type type)
{
	char			*new_txt;
	t_quote_type	*new_quoting;
	size_t			len;

	len = 0;
	if (*txt)
		len = ft_strlen(*txt);
	new_txt = malloc(len + 2);
	if (!new_txt)
		return (0);
	new_quoting = malloc(sizeof(t_quote_type) * (len + 1));
	if (!new_quoting)
		return (free(new_txt), 0);
	if (*txt)
		ft_memcpy(new_txt, *txt, len);
	new_txt[len] = c;
	new_txt[len + 1] = '\0';
	if (*quoting)
		ft_memcpy(new_quoting, *quoting, sizeof(t_quote_type) * len);
	new_quoting[len] = type;
	free(*txt);
	free(*quoting);
	*txt = new_txt;
	*quoting = new_quoting;
	return (1);
}

int	handle_squote(char *str, int *i, char **txt, t_quote_type **quoting)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		if (!append_char(txt, quoting, str[(*i)++], SINGLE))
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
		if (!append_char(txt, quoting, str[(*i)++], DOUBLE))
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
	if (!append_char(txt, quoting, str[(*i)++], NONE))
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

int	word_error(int nb)
{
	if (nb == -1)
		printf("Syntax error unclosed quotes\n");
	return (0);
}

int	lexing(t_data *data)
{
	int		i;
	int		ret;

	i = 0;
	while (data->line[i])
	{
		while (is_space(data->line[i]))
			i++;
		if (data->line[i] == 0)
			return (1);
		else if (is_operator(data->line[i]))
		{
			ret = handle_operators(&data->tokens, data->line, &i);
			if (!ret)
				return (free_token(&data->tokens), 0);
		}
		else
		{
			ret = handle_word(&data->tokens, data->line, &i);
			if (ret <= 0)
				return (free_token(&data->tokens), word_error(ret));
		}
	}
	return (1);
}
