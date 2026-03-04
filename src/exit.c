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
	if (!word || !*word || !**word)
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

void	free_token(t_token **head)
{
	t_token *tmp;

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

void	free_redirs(t_redirs **redirs)
{
	t_redirs *tmp;

	if (!redirs || !*redirs)
		return ;
	while (*redirs)
	{
		free((*redirs)->file_name);
		tmp = *redirs;
		*redirs = (*redirs)->next;
		free(tmp);
		tmp = NULL;
	}
	*redirs = NULL;
}

void	free_env(t_env **env)
{
	t_env *tmp;

	if (!env || !*env)
		return ;
	while (*env)
	{
		free((*env)->key);
		free((*env)->value);
		tmp = *env;
		*env = (*env)->next;
		free(tmp);
		tmp = NULL;
	}
	*env = NULL;
}

void	free_list(t_cmd_list **list)
{
	t_cmd_list *tmp;

	if (!list || !*list)
		return ;
	while (*list)
	{
		free_word_tab(&(*list)->args);
		free_redirs(&(*list)->redirs);
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
		tmp = NULL;
	}
	*list = NULL;
}

void	free_expanded_list(t_expanded_list **list)
{
	t_expanded_list *tmp;

	if (!list || !*list)
		return ;
	while (*list)
	{
		ft_free(&(*list)->args);
		free_redirs(&(*list)->redirs);
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
		tmp = NULL;
	}
	*list = NULL;
}

void	free_data(t_data *data)
{
	free_token(&data->tokens);
	free_list(&data->cmd_list);
	free_expanded_list(&data->expanded_list);
	free(data->exit_status);
	free(data->line);
}

void	exit_free(t_data *data, int status)
{
	free_data(data);
	free_env(&data->env);
	free(data);
	exit(status);
}

void	perror_exit_free(t_data *data, char *cmd, char *arg, int status)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free_data(data);
	free_env(&data->env);
	free(data);
	exit(status);
}

/*
EXIT CODES
Success : EXIT_SUCCESS (0)
FAILURE : EXIT_FAILURE (non zero)
CMD NOT FOUND : 127
CMD NOT EXECUTABLE (if executable (built ins or '/' in string)): 126
SYNTAX ERROR : 2
*/
