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
	if (type == IN_DIR || type == OUT_DIR || type == HEREDOC || type == APPEND)
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
	{
		res = malloc(1);
		if (!res)
			return (NULL);
		res[0] = 0;
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

t_word	*new_arg(t_token *tmp, int *flag)
{
	t_word	*ret;

	ret = malloc(sizeof(t_word));
	if (!ret)
	{
		*flag = 1;
		return (NULL);
	}
	ret->txt = ft_strdup(tmp->word->txt);
	if (!ret->txt)
	{
		*flag = 1;
		free(ret);
		return (NULL);
	}
	ret->quoting = dup_quoting(tmp->word);
	if (!ret->quoting)
	{
		*flag = 1;
		free(ret->txt);
		free(ret);
		return (NULL);
	}
	return (ret);
}

void	null_init(t_word **args, int nb_args)
{
	int	i;

	i = 0;
	while (i < nb_args)
		args[i++] = NULL;
}

static void	init_variables(t_token **tmp, t_token **prev,
							t_token *start, int *i)
{
	*tmp = start;
	*prev = NULL;
	*i = 0;
}

int	build_args(t_word **args, t_token *start, t_token *end, int *flag)
{
	t_token	*tmp;
	t_token	*prev;
	int		i;

	init_variables(&tmp, &prev, start, &i);
	while (tmp != end)
	{
		if (tmp->type == WORD && prev && is_redir(prev->type))
		{
			prev = tmp;
			tmp = tmp->next;
			continue ;
		}
		else if (tmp->type == WORD)
		{
			args[i] = new_arg(tmp, flag);
			if (!args[i])
				return (free_word_tab_2(&args, i), 0);
			i++;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

t_word	**get_args(t_token *start, t_token *end, int *flag)
{
	int			nb_args;
	t_word		**args;

	nb_args = count_args(start, end);
	args = malloc(sizeof(t_word *) * (nb_args + 1));
	if (!args)
		return (NULL);
	null_init(args, nb_args + 1);
	if (!build_args(args, start, end, flag))
		return (NULL);
	return (args);
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
