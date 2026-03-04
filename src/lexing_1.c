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

static int append_char(char **txt, t_quote_type **quoting, char c, t_quote_type type)
{
    char            *new_txt;
    t_quote_type    *new_quoting;
    size_t          len;

    len = 0;
    if (*txt)
        len = ft_strlen(*txt);
    new_txt = malloc(len + 2); // +1 char + '\0'
    if (!new_txt)
        return (0);
    new_quoting = malloc(sizeof(t_quote_type) * (len + 1));
    if (!new_quoting)
    {
        free(new_txt);
        return (0);
    }
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

int	handle_word(t_data *data, t_token **head, char *str, int *i)
{
	t_word			*word;
	char			*txt;
	t_quote_type	*quoting;
	t_token	*tmp;

	txt = NULL;
	quoting = NULL;
	while (str[*i] && !is_space(str[*i]) && !is_operator(str[*i]))
    {
        if (str[*i] == '\'')
        {
            (*i)++;
            while (str[*i] && str[*i] != '\'')
			{
				if (!append_char(&txt, &quoting, str[(*i)++], SINGLE))
				{
					free(txt);
					free(quoting);
					exit_free(data, EXIT_FAILURE);
				}
			}
            if (str[*i] != '\'')
			{
				free(txt);
				free(quoting);
            	return (1);
			}
            (*i)++;
        }
        else if (str[*i] == '"')
        {
            (*i)++;
            while (str[*i] && str[*i] != '"')
			{
            	if (!append_char(&txt, &quoting, str[(*i)++], DOUBLE))
				{
					free(txt);
					free(quoting);
					exit_free(data, EXIT_FAILURE);
				}
			}
            if (str[*i] != '"')
            {
				free(txt);
				free(quoting);
            	return (1);
			}
            (*i)++;
        }
        else
        {
            if (!append_char(&txt, &quoting, str[(*i)++], NONE))
			{
				free(txt);
				free(quoting);
				exit_free(data, EXIT_FAILURE);
			}
        }
    }
	word = malloc(sizeof(t_word));
	if (!word)
	{
		free(txt);
		free(quoting);
		exit_free(data, EXIT_FAILURE);
	}
    word->txt = txt;
    word->quoting = quoting;
	tmp = new_token(WORD, word);
	if (!tmp)
	{
		free_word(&word);
		exit_free(data, EXIT_FAILURE);
	}
	add_token(head, tmp);
	return (0);
}

void	handle_operators(t_data *data, t_token **head, char *str, int *i)
{
	t_token	*tmp;

	if (str[*i] == '|')
	{
		tmp = new_token(PIPE, NULL);
		if (!tmp)
			exit_free(data, EXIT_FAILURE);
		add_token(head, tmp);
		(*i)++;
	}
	else if (str[*i] == '<')
	{
		if (str[*i + 1] && str[*i + 1] == '<')
		{
			tmp = new_token(HEREDOC, NULL);
			if (!tmp)
				exit_free(data, EXIT_FAILURE);
			add_token(head, tmp);
			(*i) += 2;
		}
		else
		{
			tmp = new_token(IN_DIR, NULL);
			if (!tmp)
				exit_free(data, EXIT_FAILURE);
			add_token(head, tmp);
			(*i)++;
		}
	}
	else if (str[*i] == '>')
	{
		if (str[*i + 1] && str[*i + 1] == '>')
		{
			tmp = new_token(APPEND, NULL);
			if (!tmp)
				exit_free(data, EXIT_FAILURE);
			add_token(head, tmp);
			(*i) += 2;
		}
		else
		{
			tmp = new_token(OUT_DIR, NULL);
			if (!tmp)
				exit_free(data, EXIT_FAILURE);
			add_token(head, tmp);
			(*i)++;
		}
	}
}

int	lexing(t_data *data)
{
	int		i;

	i = 0;
	while (data->line[i])
	{
		while (is_space(data->line[i]))
			i++;
		if (data->line[i] == 0)
			return (1);
		else if (is_operator(data->line[i]))
			handle_operators(data, &data->tokens, data->line, &i);
		else
		{
			if (handle_word(data, &data->tokens, data->line, &i) == 1)
			{
				printf("Syntax error unclosed quotes\n");
				return (0);
			}
		}
	}
	return (1);
}
