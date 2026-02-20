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

int	count_backslash_to_rm(char *str)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '\\')
		{
			if (str[i + 1] && (str[i + 1] == '\\' || str[i + 1] == '"' || str[i + 1] == '$'))
			{
				count++;
				i++;
			}
		}
		i++;
	}
	return (count);
}

char	*modif_string(char *str)
{
	char	*res;
	int		i;
	int		j;

	res = malloc((int)strlen(str) - count_backslash_to_rm(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\')
		{
			if (str[i + 1] && (str[i + 1] == '\\' || str[i + 1] == '"' || str[i + 1] == '$'))
			{
				res[j] = str[i + 1];
				i++;
			}
			else
				res[j] = str[i];
		}
		else
			res[j] = str[i];
		j++;
		i++;
	}
	res[j] = 0;
	return (res);
}

static char	*quote(char *str, char c)
{
	int		i;
	char	*string;

	i = 1;
	while (str[i] && str[i] != c)
		i++;
	string = malloc(i);
	if (!string)
		return (NULL);
	ft_strlcpy(string, str + 1, i);
	return (string);
}

void	create_new_tokens(t_token **head)
{
	t_token	*prev;
	t_token	*last;
	char	*str;
	int		i;

	if (!head || !*head)
		return ;
	prev = NULL;
	last = *head;
	while (last->next)
	{
		prev = last;
		last = last->next;
	}
	str = ft_strdup(last->str);
	if (!str)
		exit(EXIT_FAILURE);
	if (prev)
		free_token(&prev->next);
	else
	{
		free((*head)->str);
		free(*head);
		*head = NULL;
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == 0)
			return ;
		if (str[i] == '$')
			handle_variable(head, str, &i);
		else
			handle_word_2(head, str, &i, 0);
	}
	free(str);
}

void	handle_double_quotes(t_token **head, char *str, int *i)
{
	char	*content;

	content = quote(&str[*i], '"');
	if (!content)
	{
		free_token(head);
		exit(EXIT_FAILURE);
	}
	*i += ft_strlen(content) + 2;
	add_token(head, new_token(STRING, content));
	if (dollar_exists(content))
		create_new_tokens(head);
}

void	handle_single_quotes(t_token **head, char *str, int *i)
{
	char	*content;

	content = quote(&str[*i], '\'');
	if (!content)
	{
		free_token(head);
		exit(EXIT_FAILURE);
	}
	add_token(head, new_token(S_STRING, content));
	*i += ft_strlen(content) + 2;
}

void	handle_word(t_token **head, char *str, int *i, int flag)
{
	char	*res;
	int		start;

	start = *i;
	if (flag == 1)
		(*i)++;
	while (str[*i] && str[*i] != ' ' && str[*i] != '\'' && str[*i] != '"'
			&& !is_operator(str[*i]) && str[*i] != '$')
		(*i)++;
	res = malloc(*i - start + 1);
	if (!res)
		exit(EXIT_FAILURE);
	ft_strlcpy(res, str + start, *i - start + 1);
	add_token(head, new_token(WORD, res));
}

void	handle_word_2(t_token **head, char *str, int *i, int flag)
{
	char	*res;
	int		start;

	start = *i;
	if (flag == 1)
		(*i)++;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"'
			&& !is_operator(str[*i]) && str[*i] != '$')
		(*i)++;
	res = malloc(*i - start + 1);
	if (!res)
		exit(EXIT_FAILURE);
	ft_strlcpy(res, str + start, *i - start + 1);
	add_token(head, new_token(WORD, res));
}

/*
void	merge_words(t_token **head)
{
	t_token	*tmp;
	char	*merged;

	tmp = *head;
	if (!head || !*head || !(*head)->next)
    	return ;
	while (tmp->next != NULL && tmp->next->next != NULL)
		tmp = tmp->next;
	if (tmp->str)
		merged = ft_strjoin(tmp->str, tmp->next->str);
	else
		merged = tmp->next->str;
	free(tmp->str);
	tmp->str = merged;
	tmp->type = WORD;
	if (merged != tmp->next->str)
		free(tmp->next->str);
	free(tmp->next);
	tmp->next = NULL;
}
*/


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
	char 	*res;
	int		start;

	(*i)++;
	start = *i;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '\'' || str[*i] == '"'
		|| is_operator(str[*i]))
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

void	lexing(t_token **head, char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == 0)
			return ;
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
			handle_word(head, str, &i, 0);
	}
	return ;
}
