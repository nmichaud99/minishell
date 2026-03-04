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

int	is_redir(t_token_type type)
{
	if (type ==	IN_DIR || type == OUT_DIR || type == HEREDOC || type == APPEND)
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

int	add_redir_node(t_redirs **redirs, t_token *token, t_token *end)
{
	t_redirs	*tmp;
	t_redirs	*new_node;
	char		*filename;

	new_node = malloc(sizeof(t_redirs));
	if (!new_node)
		return (0);
	if (!token->next || token->next == end || token->next->type != WORD)
	{
		new_node->file_name = NULL;
		new_node->type = 0;
		new_node->next = NULL;
	}
	else
	{
		filename = ft_strdup(token->next->word->txt);
		if (!filename)
		{
			free(new_node);
			return (0);
		}
		new_node->file_name = filename;
		new_node->type = convert_types(token->type);
		new_node->next = NULL;
	}
	if (!*redirs)
	{
		*redirs = new_node;
		return (1);
	}
	tmp = *redirs;
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (1);
}

t_redirs	*get_redirs(t_token *start, t_token *end, int *flag)
{
	t_token		*tmp;
	t_redirs 	*redirs;

	redirs = NULL;
	tmp = start;
	while (tmp != end)
	{
		if (is_redir(tmp->type))
		{
			if (add_redir_node(&redirs, tmp, end) == 0)
			{
				*flag = 1;
				return (NULL);
			}
			if (tmp->next == end || tmp->next->next == end)
				break ;
			tmp = tmp->next->next;
		}
		else
			tmp = tmp->next;
	}
	return (redirs);
}

int	count_args(t_token *start, t_token *end)
{
	t_token	*tmp;
	t_token	*prev;
	int		args_size;

	tmp = start;
	args_size = 0;
	prev = NULL;
	while (tmp != end)
	{
		if (tmp->type == WORD)
		{
			if (prev && is_redir(prev->type))
			{
				prev = tmp;
				tmp = tmp->next;
				continue ;
			}
			args_size++;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (args_size);
}

t_quote_type	*dup_quoting(t_word *word)
{
	t_quote_type	*res;
	int				size;
	int				i;

	size = ft_strlen(word->txt);
	if (size == 0)
		return (NULL);
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

t_word	**get_args(t_token *start, t_token *end, int *flag)
{
	int			nb_args;
	t_word		**args;
	t_token		*tmp;
	t_token		*prev;
	int			i;

	nb_args = count_args(start, end);
	args = malloc(sizeof(t_word *) * (nb_args + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < nb_args)
		args[i++] = NULL;
	tmp = start;
	prev = NULL;
	i = 0;
	while (tmp != end)
	{
		if (tmp->type == WORD)
		{
			if (prev && is_redir(prev->type))
			{
				prev = tmp;
				tmp = tmp->next;
				continue ;
			}
			else
			{
				args[i] = malloc(sizeof(t_word));
				if (!args[i])
				{
					*flag = 1;
					free_word_tab(&args);
					return (NULL);
				}
				args[i]->txt = ft_strdup(tmp->word->txt);
				if (!args[i]->txt)
				{
					*flag = 1;
					args[i] = NULL;
					free_word_tab(&args);
					return (NULL);
				}
				args[i]->quoting = dup_quoting(tmp->word);
				if (!args[i]->quoting)
				{
					*flag = 1;
					args[i] = NULL;
					free_word_tab(&args);
					return (NULL);
				}
				i++;
			}
		}
		prev = tmp;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

void	parsing(t_data *data)
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
				exit_free(data, EXIT_FAILURE);
			tmp_redirs = get_redirs(start, current, &flag);
			if (!tmp_redirs && flag == 1)
			{
				free_word_tab(&tmp);
				exit_free(data, EXIT_FAILURE);
			}
			tmp_list = new_cmd(tmp, tmp_redirs);
			if (!tmp_list)
			{
				free_word_tab(&tmp);
				free_redirs(&tmp_redirs);
				exit_free(data, EXIT_FAILURE);
			}
			add_cmd(&data->cmd_list, tmp_list);
			start = current->next;
		}
		current = current->next;
	}
	tmp = get_args(start, NULL, &flag);
	if (!tmp && flag == 1)
		exit_free(data, EXIT_FAILURE);
	tmp_redirs = get_redirs(start, current, &flag);
	if (!tmp_redirs && flag == 1)
	{
		free_word_tab(&tmp);
		exit_free(data, EXIT_FAILURE);
	}
	tmp_list = new_cmd(tmp, tmp_redirs);
	if (!tmp_list)
	{
		free_word_tab(&tmp);
		free_redirs(&tmp_redirs);
		exit_free(data, EXIT_FAILURE);
	}
	add_cmd(&data->cmd_list, tmp_list);
}
