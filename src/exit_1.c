/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:10:43 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 13:10:45 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(char ***str)
{
	int	i;

	if (!str || !*str)
		return ;
	i = 0;
	while ((*str)[i])
	{
		free((*str)[i]);
		(*str)[i] = NULL;
		i++;
	}
	free(*str);
	*str = NULL;
}

void	free_word(t_word **word)
{
	if (!word || !*word)
		return ;
	if ((*word)->txt)
	{
		free((*word)->txt);
		(*word)->txt = NULL;
	}
	if ((*word)->quoting)
	{
		free((*word)->quoting);
		(*word)->quoting = NULL;
	}
	free(*word);
	*word = NULL;
}

void	free_word_tab(t_word ***word)
{
	int	i;

	if (!word || !*word)
		return ;
	i = 0;
	while ((*word)[i])
	{
		free_word(&((*word)[i]));
		i++;
	}
	free(*word);
	*word = NULL;
}

void	free_word_tab_2(t_word ***word, int size)
{
	int	i;

	if (!word || !*word)
		return ;
	i = 0;
	while (i < size)
	{
		free_word(&((*word)[i]));
		i++;
	}
	free(*word);
	*word = NULL;
}

void	free_token(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		free_word(&(*head)->word);
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
		tmp = NULL;
	}
	*head = NULL;
}

/* 
EXIT CODES 
Success : EXIT_SUCCESS (0)
FAILURE : EXIT_FAILURE (non zero)
CMD NOT FOUND : 127
CMD NOT EXECUTABLE (if executable (built ins or '/' in string)): 126
SYNTAX ERROR : 2
*/
