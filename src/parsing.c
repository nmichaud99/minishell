/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 13:22:20 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/27 13:22:24 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_redir_node(t_redirs **redirs, t_token *token)
{
	t_redirs	*tmp;
	t_redirs	*new_node;
	char		*filename;

	new_node = malloc(sizeof(t_redirs));
	if (!new_node)
		return (0);
	filename = ft_strdup(token->next->word->txt);
	if (!filename)
		return (free(new_node), 0);
	new_node->file_name = filename;
	new_node->type = convert_types(token->type);
	if (token->next->word->quoting[0] == SINGLE || token->next->word->quoting[0] == DOUBLE)
		new_node->to_expand = 0;
	else
		new_node->to_expand = 1;
	new_node->next = NULL;
	if (!*redirs)
	{
		*redirs = new_node;
		return (1);
	}
	tmp = *redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (1);
}

t_redirs	*get_redirs(t_token *start, t_token *end, int *flag)
{
	t_token		*tmp;
	t_redirs	*redirs;

	redirs = NULL;
	tmp = start;
	while (tmp != end)
	{
		if (is_redir(tmp->type))
		{
			if (add_redir_node(&redirs, tmp) == 0)
			{
				*flag = 1;
				return (NULL);
			}
			if (!tmp->next->next || tmp->next->next == end)
				break ;
			tmp = tmp->next->next;
		}
		else
			tmp = tmp->next;
	}
	return (redirs);
}

t_quote_type	*dup_quoting(t_word *word)
{
	t_quote_type	*res;
	int				size;
	int				i;

	size = ft_strlen(word->txt);
	if (size == 0)
	{
		res = malloc(sizeof(t_quote_type));
		if (!res)
			return (NULL);
		res[0] = word->quoting[0];
		return (res);
	}
	res = malloc(sizeof(t_quote_type) * size);
	if (!res)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i] = word->quoting[i];
		i++;
	}
	return (res);
}

t_cmd_list	*new_cmd_node(t_token *start, t_token *current)
{
	t_cmd_list	*ret;
	t_word		**tmp_word;
	t_redirs	*tmp_redirs;
	int			flag;

	flag = 0;
	tmp_word = get_args(start, current, &flag);
	if (!tmp_word && flag == 1)
		return (0);
	tmp_redirs = get_redirs(start, current, &flag);
	if (!tmp_redirs && flag == 1)
	{
		free_word_tab(&tmp_word);
		return (NULL);
	}
	ret = new_cmd(tmp_word, tmp_redirs);
	if (!ret)
	{
		free_word_tab(&tmp_word);
		free_redirs(&tmp_redirs);
		return (NULL);
	}
	return (ret);
}

int	parsing(t_data *data)
{
	t_cmd_list	*tmp_cmd_list;
	t_token		*start;
	t_token		*current;

	start = data->tokens;
	current = data->tokens;
	while (current)
	{
		if (current->type == PIPE)
		{
			tmp_cmd_list = new_cmd_node(start, current);
			if (!tmp_cmd_list)
				return (free_list(&data->cmd_list), 0);
			add_cmd(&data->cmd_list, tmp_cmd_list);
			start = current->next;
		}
		current = current->next;
	}
	tmp_cmd_list = new_cmd_node(start, NULL);
	if (!tmp_cmd_list)
		return (free_list(&data->cmd_list), 0);
	add_cmd(&data->cmd_list, tmp_cmd_list);
	return (1);
}

/*int	parsing(t_data *data)
{
	t_token		*start;
	t_token		*current;
	t_word		**tmp;
	t_redirs	*tmp_redirs;
	t_cmd_list	*tmp_list;
	int			flag;

	flag = 0;
	start = data->tokens;
	current = data->tokens;
	while (current)
	{
		if (current->type == PIPE)
		{
			tmp = get_args(start, current, &flag);
			if (!tmp && flag == 1)
			{
				free_list(&data->cmd_list);
				data->cmd_list = NULL;
				return (0);
			}
			tmp_redirs = get_redirs(start, current, &flag);
			if (!tmp_redirs && flag == 1)
			{
				free_word_tab(&tmp);
				free_list(&data->cmd_list);
				data->cmd_list = NULL;
				return (0);
			}
			tmp_list = new_cmd(tmp, tmp_redirs);
			if (!tmp_list)
			{
				free_word_tab(&tmp);
				free_redirs(&tmp_redirs);
				free_list(&data->cmd_list);
				data->cmd_list = NULL;
				return (0);
			}
			add_cmd(&data->cmd_list, tmp_list);
			start = current->next;
		}
		current = current->next;
	}
	tmp = get_args(start, NULL, &flag);
	if (!tmp && flag == 1)
	{
		free_list(&data->cmd_list);
		data->cmd_list = NULL;
		return (0);
	}
	tmp_redirs = get_redirs(start, current, &flag);
	if (!tmp_redirs && flag == 1)
	{
		free_word_tab(&tmp);
		free_list(&data->cmd_list);
		data->cmd_list = NULL;
		return (0);
	}
	tmp_list = new_cmd(tmp, tmp_redirs);
	if (!tmp_list)
	{
		free_word_tab(&tmp);
		free_redirs(&tmp_redirs);
		free_list(&data->cmd_list);
		data->cmd_list = NULL;
		return (0);
	}
	add_cmd(&data->cmd_list, tmp_list);
	return (1);
}*/
