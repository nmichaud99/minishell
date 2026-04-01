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
# include <sys/stat.h>
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
	SINGLE,
	SPECIAL
}	t_quote_type;

typedef enum e_builtin_type
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	NO
}	t_builtin_type;

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
	t_word			*filename;
	int				to_expand;
	struct s_redirs	*next;
}	t_redirs;

typedef struct s_expanded_redirs
{
	t_redir_type				type;
	char						*file_name;
	char						*heredoc_name;
	int							to_expand;
	struct s_expanded_redirs	*next;
}	t_expanded_redirs;

typedef struct s_cmd_list
{
	t_word				**args;
	t_redirs			*redirs;
	struct s_cmd_list	*next;
}	t_cmd_list;

typedef struct s_expanded_list
{
	char							**args;
	t_expanded_redirs				*expanded_redirs;
	struct s_expanded_list			*next;
}	t_expanded_list;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				has_value;
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
	int				exit_status;
	int				pipefd[2];
	pid_t			pid;
	pid_t			last_pid;
	int				last_status;
	char			*full_path;
}	t_data;

extern volatile 		sig_atomic_t g_SignalStatus;

// --- utils --- //

// utils
t_token			*new_token(t_token_type type, t_word *word);
void			add_token(t_token **head, t_token *new);
t_cmd_list		*new_cmd(t_word **args, t_redirs *redirs);
void			add_cmd(t_cmd_list **list, t_cmd_list *new);
int				ft_strcmp(const char *s1, const char *s2);
// error messages
void			ft_perror(char **arg);
void			ft_perror_2(char *s1, char *s2, char *s3);
void			f_printf(char *s1, char *s2);
void			f_printf_2(char *s1, char *s2, char *s3);
void			f_printf_3(char *s1, char *s2, char *s3);
// --- env utils --- //
void			print_env(t_data *data);
char			**malloc_env_tab(t_data *data);
char			**get_env_tab(t_data *data);
int				build_minimal_env(t_data *data);

// --- exit and free --- //

// exit_1
void			ft_free(char ***str);
void			free_word(t_word **word);
void			free_word_tab(t_word ***word);
void			free_word_tab_2(t_word ***word, int size);
void			free_token(t_token **head);
// exit_2
void			free_redirs(t_redirs **redirs);
void			free_expanded_redirs(t_expanded_redirs **redirs);
void			free_env(t_env **env);
void			free_list(t_cmd_list **list);
void			free_expanded_list(t_expanded_list **list);
void			free_data(t_data *data);
// exit_3
void			exit_free(t_data *data, int status);
void			error_sys(t_data *data, char *s);

// --- lexing --- //

// lexing_utils
int				is_operator(char c);
int				is_space(char c);
int				word_error(int nb);
int				lex_append_char(char **txt, t_quote_type **quoting,
							char c, t_quote_type type);
// lexing_handle_operators
int				handle_pipe(t_token **head, int *i);
int				handle_indir(t_token **head, char *str, int *i);
int				handle_outdir(t_token **head, char *str, int *i);
int				handle_operators(t_token **head, char *str, int *i);
// lexing_handle_words
int				handle_squote(char *str, int *i, char **txt, t_quote_type **quoting);
int				handle_dquote(char *str, int *i, char **txt, t_quote_type **quoting);
int				handle_char(char *str, int *i, char **txt, t_quote_type **quoting);
int				build_word(char *str, int *i, char **txt, t_quote_type **quoting);
int				handle_word(t_token **head, char *str, int *i);
// lexing
int				lexing(t_data *data);
// syntax check
int				syntax_check(t_data *data);

// --- parsing --- //

// parsing_utils
int				is_redir(t_token_type type);
t_redir_type	convert_types(t_token_type token_type);
void			null_init(t_word **args, int nb_args);
void			fill_redir_node(t_redirs **redir, t_word *filename, t_token_type type, t_quote_type *quoting);
// parsing_args
int				count_args(t_token *start, t_token *end);
t_word			*new_arg(t_token *tmp, int *flag);
int				build_args(t_word **args, t_token *start, t_token *end, int *flag);
t_word			**get_args(t_token *start, t_token *end, int *flag);
// parsing
int				add_redir_node(t_redirs **redirs, t_token *token);
t_redirs		*get_redirs(t_token *start, t_token *end, int *flag);
t_quote_type	*dup_quoting(t_word *word);
t_cmd_list		*new_cmd_node(t_token *start, t_token *current);
int				parsing(t_data *data);

// --- expansion --- //

// expansion_utils
char			*get_exit_code(t_data *data);
char			*get_variable_value(t_data *data, char *str);
int				type_of_char(char c, t_quote_type quoting, t_quote_type quote);
int				append_variable(char **res, char **str);
int				append_char(char **res, char c);
// expansion_args
char			*expand_word(t_data *data, t_word *arg, int *i, t_quote_type quote);
char			*expand_arg(t_data *data, t_word *arg);
char			**get_expanded_args(t_data *data, t_cmd_list *lst);
// expansion
t_expanded_redirs		*dup_redirs(t_data *data, t_redirs *src);
t_expanded_list	*build_expanded_list(t_data *data, char **expanded_args, t_cmd_list *lst);
int				expansion(t_data *data);

// --- built-ins --- //

// utils
t_builtin_type	is_built_in(char *arg);
int				exec_cmd(t_data *data, t_expanded_list *list);
void			save_std_fds(t_data *data, int *saved_stdin, int *saved_stdout);
int				redir_handler(t_data *data, t_expanded_list *list);
int				exec_built_in(t_data *data, t_expanded_list *list, int flag);
// unset
void			free_env_node(t_env **env);
void			delete_node_if(char *arg, t_data *data);
int				exec_unset(t_data *data, char **args);
// export_1
char			*get_variable_key(const char *s);
void			print_env_export(t_data *data);
int				is_valid_string(char *str, int has_value);
int				find_key(t_data *data, char *key);
int				ft_schr(const char *s, char c);
// export_2
int				create_node(t_env **new_node, char *env_line, int has_value);
int				add_env_node(t_data *data, char *env_line, int has_value);
int				looper(t_data *data, char *new_key, char *new_value);
int				add_or_modify_env_node(t_data *data, char *new_var, int has_value);
int				exec_export(t_data *data, char **args);
// echo
void			print_args(int option_n, char **args);
int				exec_echo(char **args);
// env
int				exec_env(t_data *data, char **args);
// pwd_cd
int				exec_pwd(t_data *data, char **args);
int				exec_cd(t_data *data, char **args);
// exit
int				exec_exit(t_data *data, char **args);

// --- pipes and exec

// heredoc_handler
int				heredoc_handler(t_data *data);
// redirs handler
int				redir_in_handler(t_data *data, t_expanded_list *list);
int				redir_out_handler(t_expanded_list *list);
char			*expand_line(t_data *data, char *line);
// pipes
void			exec_if(t_data *data, int *prev_fd, t_expanded_list *list);
void			close_if(t_data *data, int *prev_fd, t_expanded_list *list);
int				pipe_creator(t_data *data, int *prev_fd, t_expanded_list *list);
// get path
char			*build_path(char *cmd, char *path);
int				access_path(char *path, int *permission_flag, char **tmp);
char			*get_full_path(char *cmd, char **path);
char			*find_cmd(char *cmd, char **path);
// exec_commands_1
void			ft_exec(t_data *data, t_expanded_list *list);
int			redir_in(t_data *data, t_expanded_list *list, int prev_fd, int first);
char			**get_all_paths(t_data *data);
void			handle_path_error(t_data *data, int c, char *m, t_expanded_list *list);
void			get_path_and_exec(t_data *data, t_expanded_list *list);
// exec_commands_2
void			exec_cmd1(t_data *data, t_expanded_list *list);
void			exec_cmdn(t_data *data, t_expanded_list *list, int prev_fd);
void			exec_last_cmd(t_data *data, t_expanded_list *list, int prev_fd);

// main
void			sigint_handler(int sig);

#endif
