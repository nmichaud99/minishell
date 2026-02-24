/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:29:17 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/23 16:29:19 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void	create_node(t_token **tokens)
{
	t_token	*current;
	//t_ast	**res;

	current = *tokens;
	while (current && current->type != PIPE)
	{
		create_leaf_node(current);
		current = current->next;
	}
	if (current && current->type == PIPE)
		create_pipe_node(&current);
}

t_ast	*create_pipe_node(t_token **tokens, int last_pipe)
{
	t_ast	*res;
	t_token	*current;
	t_token	**right;
	int		i;

	res = malloc(sizeof(t_ast));
	if (!res)
		return (NULL);
	i = 0;
	res->type = NODE_PIPE;
	res->cmd = NULL;
	current = *tokens;
	while (current->next != NULL && i != last_pipe - 1)
	{
		current = current->next;
		i++;
	}
	printf("LAST PIPE FOUND AT INDEX : %d\n", i);
	right = &current->next->next;
	current->next = NULL;
	res->left = create_node(tokens);
	res->right = create_node(right);
	return (res);
}

t_ast	*create_leaf_node(t_token *cmd)
{
	t_ast	*res;

	res = malloc(sizeof(t_ast));
	if (!res)
		return (NULL);
	res->type = NODE_CMD;
	res->cmd = cmd;
	while (res->cmd->next != NULL && res->cmd->type != PIPE && res->cmd->type)
		cmd = cmd->next;
	res->cmd->next = NULL;
	res->left = NULL;
	res->right = NULL;
	return (res);
}

int	find_last_pipe(t_token **tokens)
{
	t_ast	*test;
	t_token	*token;
	int		i;
	int		last_pipe;

	i = 0;
	last_pipe = -1;
	token = *tokens;
	while (token)
	{
		if (token->str)
			printf("token str : %s\n", token->str);
		if (token->type == PIPE)
			last_pipe = i;
		token = token->next;
		i++;
	}
	return (last_pipe);
}

t_ast	*ft_ast(t_token **tokens)
{
	int		i;
	t_ast	*test;

	i = find_last_pipe(tokens);
	printf("last pipe index : %d\n", i);
	if (i == -1)
	{
		create_leaf_node(*tokens);
		return ;
	}
	test = create_pipe_node(tokens, i);
	if (test->cmd->str)
	{
		printf("test : %s\n", test->cmd->str);
	}
}
 */

// V2

int	is_arg(t_token_type type)
{
	if (type != PIPE && type != WORD && type != STRING && type != S_STRING
		&& type != VARIABLE)
		return (0);
	return (1);
}

int	is_redir(t_token_type type)
{
	if (type ==	IN_DIR || type == OUT_DIR || type == HEREDOC || type == APPEND)
		return (1);
	return (0);
}

int	count_args(t_token **tokens)
{
	t_token	*tmp;
	int		args_size;
	int		i;

	tmp = *tokens;
	i = 0;
	args_size = 0;
	while (tmp != NULL && !is_arg(tmp->type))
	{
		i++;
		tmp = tmp->next;
	}
	while (tmp != NULL && is_arg(tmp->type))
	{
		args_size++;
		tmp = tmp->next;
	}
	return (args_size);
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

void	add_redir_node(t_redirs **redirs, t_token_type type, char *file_name)
{
	t_redirs	*tmp;
	t_redirs	*new_node;

	new_node = malloc(sizeof(t_redirs));
	if (!new_node)
		return ;
	new_node->file_name = file_name;
	new_node->type = convert_types(type);
	new_node->next = NULL;
	tmp = *redirs;
	if (!tmp)
	{
		tmp = new_node;
		return ;
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

t_redirs	*get_redirs(t_token **tokens, t_redirs **redirs)
{
	t_token		**head;
	t_token		*tmp;

	head = tokens;
	tmp = *tokens;
	while (tmp != NULL)
	{
		if (tmp->next && (tmp->next->type == IN_DIR
			|| tmp->next->type == HEREDOC) && is_arg(tmp->type))
		{
			add_redir_node(redirs, tmp->next->type, tmp->str);
			tmp = tmp->next->next;
			continue ;
		}
		if (tmp->next && (tmp->type == OUT_DIR || tmp->type == APPEND)
			&& is_arg(tmp->next->type))
		{
			add_redir_node(redirs, tmp->type, tmp->next->str);
			tmp = tmp->next->next;
			continue ;
		}
		tmp = tmp->next;
	}
	return (*redirs);
}

char	**get_args(t_token **tokens)
{
	int			nb_args;
	char		**args;
	t_token		*tmp;
	int			i;

	nb_args = count_args(tokens);
	args = malloc(sizeof(char *) * (nb_args + 1));
	if (!args)
		return (NULL);
	tmp = *tokens;
	i = 0;
	while (tmp != NULL && !is_arg(tmp->type))
		tmp = tmp->next;
	while (tmp != NULL && is_arg(tmp->type))
	{
		args[i] = ft_strdup(tmp->str);
		tmp = tmp->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

t_cmd_list	*create_node(t_token **tokens, t_cmd_list **list)
{
	t_cmd_list		*tmp;
	t_redirs		*redirs;
	t_cmd_list		*new_node;

	redirs = malloc(sizeof(t_redirs));
	if (!redirs)
		return (NULL);
	new_node = malloc(sizeof(t_cmd_list));
	if (!new_node)
		return (NULL);
	new_node->redirs = get_redirs(tokens, &redirs);
	new_node->args = get_args(tokens);
	new_node->next = NULL;
	tmp = *list;
	if (!*list)
	{
		*list = new_node;
		return (*list);
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (tmp);
}

t_cmd_list	*use_tokens(t_token **tokens)
{
	t_token		**head;
	t_token		*tmp;
	t_token		*pipe_token;
	t_token		*next_start;
	t_cmd_list	*list;


	list = NULL;
	head = tokens;
	tmp = *tokens;
	while (tmp != NULL)
	{
		if (tmp->next && tmp->next->type == PIPE)
		{
			pipe_token = tmp->next;
			if (tmp->next->next)
				next_start = tmp->next->next;
			tmp->next = NULL;
			create_node(head, &list);
			head = &next_start;
			tmp = next_start;
			free(pipe_token);
			continue ;
		}
		tmp = tmp->next;
	}
	create_node(head, &list);
	return (list);
}
