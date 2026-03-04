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
	PIPE,
	IN_DIR,
	OUT_DIR,
	HEREDOC,
	APPEND,
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC,
	REDIR_APPEND,
}	t_redir_type;

typedef enum e_quote_type
{
	NONE,
	DOUBLE,
	SINGLE
}	t_quote_type;

typedef struct s_word
{
	char			*txt;
	t_quote_type	*quoting;
}	t_word;

typedef struct s_token
{
	t_token_type	type;
	t_word			*word;
	struct s_token	*next;
}	t_token;

typedef struct s_redirs
{
	t_redir_type	type;
	char			*file_name;
	struct s_redirs	*next;
}	t_redirs;

// Command List
typedef struct s_cmd_list
{
	t_word			**args;
	t_redirs		*redirs;
	//t_token_type	type;
	struct s_cmd_list	*next;
}	t_cmd_list;

typedef struct s_expanded_list
{
	char					**args;
	t_redirs				*redirs;
	struct s_expanded_list	*next;
}	t_expanded_list;

// Environment
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct	s_data
{
	char			*line;
	t_token			*tokens;
	t_cmd_list		*cmd_list;
	t_expanded_list	*expanded_list;
	t_env			*env;
	char			**env_tab;
	int				*exit_status;
	int				pipefd[2];
	pid_t			pid;
	pid_t			last_pid;
	int				last_status;
	char			*full_path;
}	t_data;

// --- utils --- //
t_token			*new_token(t_token_type type, t_word *word);
void			add_token(t_token **head, t_token *new);
t_cmd_list		*new_cmd(t_word **args, t_redirs *redirs);
void			add_cmd(t_cmd_list **list, t_cmd_list *new);
int				ft_strcmp(const char *s1, const char *s2);

// --- env utils --- //
char			**get_env_tab(t_data *data);

// --- exit and free --- //

void			ft_free(char ***str);
void			free_word(t_word **word);
void			free_token(t_token **head);
void			free_env(t_env **env);
void			free_redirs(t_redirs **redirs);
void			free_word_tab(t_word ***word);
void			free_list(t_cmd_list **list);
void			free_expanded_list(t_expanded_list **list);
void			free_data(t_data *data);
void			exit_free(t_data *data, int status);
void			error_sys(t_data *data, char *s);

// --- lexing --- //

// lexing_1
int				handle_word(t_data *data, t_token **head, char *str, int *i);
void			handle_operators(t_data *data, t_token **head, char *str, int *i);
int				lexing(t_data *data);
// lexing_2
int				is_operator(char c);
int				is_space(char c);
// syntax check
int				syntax_check(t_data *data);

// --- parsing --- //

int				is_redir(t_token_type type);
t_redir_type	convert_types(t_token_type token_type);
int				add_redir_node(t_redirs **redirs, t_token *token, t_token *end);
t_redirs		*get_redirs(t_token *start, t_token *end, int *flag);
int				count_args(t_token *start, t_token *end);
t_quote_type	*dup_quoting(t_word *word);
t_word			**get_args(t_token *start, t_token *end, int *flag);
void			parsing(t_data *data);

// --- expansion --- //
char			*get_variable_value(t_data *data, char *str);
char			*expand(t_data *data, t_word *arg, int *i, t_quote_type quote);
char			*expand_arg(t_data *data, t_word *arg);
t_redirs		*dup_redirs(t_redirs *src);
void			expansion(t_data *data);

// --- built-ins --- //

// unset
void			free_env_node(t_env **env);
int				exec_unset(t_data *data, char **args);
// export
char			*get_variable_key(const char *s);
void			add_env_node(t_data *data, char *env_line);
void			add_or_modify_env_node(t_data *data, char *new_var);
void			print_env_export(t_data *data);
int				is_valid_string(char *str);
int				ft_export(t_data *data, char **args);
// echo
int				ft_echo(char **args);
// env
void			print_env(t_data *data);
// cd
int				exec_cd(t_data *data, char **args);

// --- pipes and exec

// redirs handler
int				redir_in_handler(t_data *data, t_expanded_list *list);
int				redir_out_handler(t_data *data, t_expanded_list *list);
// pipes
void			exec_if(t_data *data, int *prev_fd, t_expanded_list *list);
int				pipe_creator(t_data *data, int *prev_fd, t_expanded_list *list);
// get path
char			*find_cmd(t_data *data, char *cmd, char **path, int *err);
// exec commands
int				exec_cmd1(t_data *data, t_expanded_list *list);
int				exec_cmdn(t_data *data, t_expanded_list *list, int prev_fd);
int				exec_last_cmd(t_data *data, t_expanded_list *list, int prev_fd);

#endif
