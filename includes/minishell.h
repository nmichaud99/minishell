/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 14:47:45 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 14:47:46 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>
# include "../libft/libft.h"

typedef enum e_token_type
{
	WORD,
	STRING,
	S_STRING,
	PIPE,
	IN_DIR,
	OUT_DIR,
	HEREDOC,
	APPEND,
	VARIABLE
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC,
	REDIR_APPEND,
}	t_redir_type;

typedef struct s_token
{
	t_token_type	type;
	char			*str;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_PIPE,
	NODE_REDIR,
	NODE_CMD
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	t_token			*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct	s_data
{
	t_token	*tokens;
	t_ast	*nodes;
	char	*line;
}	t_data;

typedef struct s_redirs
{
	t_redir_type	type;
	char			*file_name;
	struct s_redirs	*next;
}	t_redirs;

// AST v2
typedef struct s_cmd_list
{
	char			**args;
	t_redirs		*redirs;
	//t_token_type	type;
	struct s_cmd_list	*next;
}	t_cmd_list;

// utils
void	free_token(t_token **head);
t_token	*new_token(t_token_type type, char *content);
void	add_token(t_token **head, t_token *new);

// exit
void	exit_free(t_data *data, int status);

// lexing
char	*dquote(t_token **head, char *str, char c);
int		is_operator(char c);
int		dollar_exists(char *str);
int		backslash_exists(char *str);
void	handle_word(t_token **head, char *str, int *i, int flag);
void	handle_word_2(t_token **head, char *str, int *i, int flag);
void	handle_operators(t_token **head, char *str, int *i);
void	handle_variable(t_token **head, char *str, int *i);
void	handle_semi(t_token **head, char *str, int *i);
void	lexing(t_data *data);
int		del_exists(char *str);
void	create_new_tokens_del(t_token **head);

// Cmd List
t_cmd_list	*use_tokens(t_token **tokens);
t_cmd_list	*create_node(t_token **tokens, t_cmd_list **list);

#endif
