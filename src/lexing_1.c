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

char	*quote(char *str, char c, t_token *head)
{
	int		i;
	char	*string;

	i = 1;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] != c)
		return (NULL);
	else
	{
		string = malloc(i);
		if (!string)
		{
			free_token(&head);
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(string, str + 1, i);
		return (string);
	}
}

t_token	*new_token(t_token_type type, char *content)
{
	t_token	*res;

	res = malloc(sizeof(t_token));
	if (!res)
		return (NULL);
	res->type = type;
	res->str = content;
	res->next = NULL;
	return (res); 
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	handle_double_quotes(t_token **head, char *str, int *i)
{
	char	*content;

	content = quote(&str[*i], '"', *head);
	if (!content)
	{
		content = dquote(head, str, '"');
		free_token(head);
		*i = -1;
		lexing(head, content);
		free(content);
		return ;
	}
	add_token(head, new_token(STRING, content));	
	*i += ft_strlen(content) + 2;
}

void	handle_single_quotes(t_token **head, char *str, int *i)
{
	char	*content;

	content = quote(&str[*i], '\'', *head);
	if (!content)
	{
		content = dquote(head, str, '\'');
		free_token(head);
		*i = -1;
		lexing(head, content);
		free(content);
		return ;
	}
	add_token(head, new_token(S_STRING, content));
	*i += ft_strlen(content) + 2;
}

void	handle_operators(t_token **head, char *str, int *i)
{
	if (str[*i] == '|')
		add_token(head, new_token(PIPE, NULL));
	else if (str[*i] == '<')
	{
		if (str[*i + 1] && str[*i + 1] == '<')
		{
			add_token(head, new_token(HEREDOC, NULL));
			(*i)++;
		}
		else
			add_token(head, new_token(IN_DIR, NULL));
	}
	else if (str[*i] == '>')
	{
		if (str[*i + 1] && str[*i + 1] == '>')
		{
			add_token(head, new_token(APPEND, NULL));
			(*i)++;
		}
		else
			add_token(head, new_token(OUT_DIR, NULL));
	}
	(*i)++;
}

void	handle_variable(t_token **head, char *str, int *i)
{
	char *res;
	int		start;

	(*i)++;
	start = *i;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '\'' || str[*i] == '"')
	{
		res = malloc(2);
		if (!res)
			exit(EXIT_FAILURE);
		res[0] = '$';
		res[1] = 0;
		add_token(head, new_token(WORD, res));
		return ;
	}
	while (str[*i] && str[*i] != ' ' && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	res = malloc(*i - start + 1);
	if (!res)
		exit(EXIT_FAILURE);
	ft_strlcpy(res, str + start, *i - start + 1);
	add_token(head, new_token(VARIABLE, res));
}

void	handle_word(t_token **head, char *str, int *i)
{
	char	*res;
	int		start;

	start = *i;
	while (str[*i] && str[*i] != ' ' && str[*i] != '\'' && str[*i] != '"' && !is_operator(str[*i]))
		(*i)++;
	res = malloc(*i - start + 1);
	if (!res)
		exit(EXIT_FAILURE);
	ft_strlcpy(res, str + start, *i - start + 1);
	add_token(head, new_token(WORD, res));
}

void	lexing(t_token **head, char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == '"')
		{
			handle_double_quotes(head, str, &i);
			if (i == -1)
				return ;
		}
		else if (str[i] == '\'')
		{
			handle_single_quotes(head, str, &i);
			if (i == -1)
				return ;
		}
		else if (is_operator(str[i]))
			handle_operators(head, str, &i);
		else if (str[i] == '$')
			handle_variable(head, str, &i);
		else
			handle_word(head, str, &i);
	}
	return ;
}
