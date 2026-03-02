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

int	is_arg(t_token_type type)
{
	if (type != WORD && type != STRING && type != S_STRING && type != VARIABLE)
		return (0);
	return (1);
}

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

void	add_redir_node(t_data *data, t_redirs **redirs, t_token *token, t_token *end)
{
	t_redirs	*tmp;
	t_redirs	*new_node;
	char		*filename;

	new_node = malloc(sizeof(t_redirs));
	if (!new_node)
		return ;
	if (!token->next || token->next == end)
	{
		new_node->file_name = NULL;
		new_node->type = 0;
		new_node->next = NULL;
	}
	else
	{
		if (token->type == VARIABLE)
			filename = getenv(token->next->str);
		else
		{
			filename = ft_strdup(token->next->str);
			if (!filename)
				exit_free(data, EXIT_FAILURE);
		}
		new_node->file_name = filename;
		new_node->type = convert_types(token->next->type);
		new_node->next = NULL;
	}
	if (!*redirs)
	{
		*redirs = new_node;
		return ;
	}
	tmp = *redirs;
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

t_redirs	*get_redirs(t_data *data, t_token *start, t_token *end)
{
	t_token		*tmp;
	t_redirs 	*redirs;

	redirs = NULL;
	tmp = start;
	while (tmp != end)
	{
		if (is_redir(tmp->type))
		{
			add_redir_node(data, &redirs, tmp, end);
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
		if (is_arg(tmp->type))
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

char	**get_args(t_data *data, t_token *start, t_token *end)
{
	int			nb_args;
	char		**args;
	t_token		*tmp;
	t_token		*prev;
	int			i;

	nb_args = count_args(start, end);
	args = malloc(sizeof(char *) * (nb_args + 1));
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
		if (is_arg(tmp->type))
		{
			if (prev && is_redir(prev->type))
			{
				prev = tmp;
				tmp = tmp->next;
				continue ;
			}
			if (tmp->type == VARIABLE)
				printf("On a une variable donc pas gere !\n");
			else
			{
				args[i] = ft_strdup(tmp->str);
				if (!args[i])
					exit_free(data, EXIT_FAILURE);
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

	start = data->tokens;
	current = data->tokens;
	while (current)
	{
		if (current->type == PIPE)
		{
			add_cmd(&data->cmd_list, new_cmd(get_args(data, start, current), get_redirs(data, start, current)));
			start = current->next;
		}
		current = current->next;
	}
	add_cmd(&data->cmd_list, new_cmd(get_args(data, start, NULL), get_redirs(data, start, NULL)));
}
