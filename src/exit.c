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

void	free_token(t_token **head)
{
	t_token *tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		free((*head)->str);
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

void	free_list(t_cmd_list **list)
{
	t_cmd_list *tmp;

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
	free(data->line);
}

void	exit_free(t_data *data, int status)
{
	free_data(data);
	free(data);
	exit(status);
}
