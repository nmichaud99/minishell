/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:40:16 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 16:40:17 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(t_token_type type, t_word *word)
{
	t_token	*res;

	res = malloc(sizeof(t_token));
	if (!res)
		return (NULL);
	res->type = type;
	res->word = word;
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

t_cmd_list	*new_cmd(t_word **args, t_redirs *redirs)
{
	t_cmd_list	*res;

	res = malloc(sizeof(t_cmd_list));
	if (!res)
		return (NULL);
	res->args = args;
	res->redirs = redirs;
	res->next = NULL;
	return (res);
}

void	add_cmd(t_cmd_list **list, t_cmd_list *new)
{
	t_cmd_list	*tmp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if ((unsigned char)s1[i] == (unsigned char)s2[i])
			i++;
		else
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	}
	return (0);
}

void	close_saved_stds(t_data *data)
{
	if (data->saved_stdin != -1)
		close(data->saved_stdin);
	if (data->saved_stdout != -1)
		close(data->saved_stdout);
}

void	dup_and_close(t_data *data, int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		close(fd1);
		error_sys(data, "dup2 error 2");
	}
	close(fd1);
}
