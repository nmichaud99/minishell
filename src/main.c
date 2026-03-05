/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:08:06 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/16 16:08:07 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t gSignalStatus = 0;

void	init_env_tab(char **env, t_data *data)
{
	int	i;

	i = 0;
	while (env[i])
	{
		add_env_node(data, env[i]);
		i++;
	}
}

void	init_data(t_data *data, int flag)
{
	if (flag == 0)
	{
		data->line = NULL;
		data->tokens = NULL;
		data->cmd_list = NULL;
		data->expanded_list = NULL;
		data->env = NULL;
		data->env_tab = NULL;
		data->exit_status = malloc(sizeof(int));
		if (!data->exit_status)
			exit(EXIT_FAILURE);
		*(data->exit_status) = 0;
		data->full_path = NULL;
	}
	else
	{
		free_token(&data->tokens);
		free_list(&data->cmd_list);
		free_expanded_list(&data->expanded_list);
		ft_free(&data->env_tab);
		free(data->full_path);
		data->full_path = NULL;
		free(data->line);
		data->line = NULL;
	}
}

static void	wait_and_return(t_data *data)
{
	int	status;
	int	i;

	*(data->exit_status) = 0;
	data->last_status = 0;
	i = 1;
	while (i > 0)
	{
		i = wait(&status);
		if (i == data->last_pid)
			data->last_status = status;
	}
	if (WIFEXITED(data->last_status))
		*(data->exit_status) = (WEXITSTATUS(data->last_status));
}

void	sigint_handler(int sig)
{
	gSignalStatus = sig;
	write(1, "\n", 1);
	rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	t_data			*data;
	t_expanded_list	*list;
	int				prev_fd;

	(void)av;
	if (ac != 1)
		return (0);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	init_data(data, 0);
	init_env_tab(env, data);
	while (1)
	{
		init_data(data, 1);
		gSignalStatus = 0;
		data->line = readline("minishell$ ");
		if (!data->line)
		{
			printf("exit\n");
			break ;
		}
		if (*(data->line))
			add_history(data->line);
		if (gSignalStatus == SIGINT)
		{
			free(data->line);
			continue ;
		}
		if (!lexing(data))
		{
			free_data(data);
			continue ;
		}
		if (!syntax_check(data))
		{
			printf("Syntax error !\n");
			free_data(data);
			continue ;
		}
		parsing(data);
		if (!data->cmd_list)
			exit_free(data, EXIT_FAILURE);
		expansion(data);
		data->env_tab = get_env_tab(data);
		prev_fd = -1;
		list = data->expanded_list;
		/*if (!*list->args && *(list->redirs) == NULL)
			break ;*/
		if (data->expanded_list->next || !*(list->args) || is_built_in(*list->args) == NO)
		{
			while (list)
			{
				data->last_pid = pipe_creator(data, &prev_fd, list);
				list = list->next;
			}
			wait_and_return(data);
		}
		else
			*(data->exit_status) = exec_built_in(data, list, 0);
	}
	free_env(&data->env);
	free(data->exit_status);
	free(data);
	rl_clear_history();
	return (0);
}

/*int	main(int ac, char **av, char **env)
{
	t_data			*data;
	t_expanded_list	*list;

	(void)ac;
	(void)av;
	(void)env;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	init_data(data, 0);
	init_env_tab(env, data);
	while (1)
	{
		init_data(data, 1);
		gSignalStatus = 0;
		data->line = readline("minishell$ ");
		if (!data->line)
		{
			free(data->exit_status);
			printf("exit\n");
			break ;
		}
		if (*(data->line))
			add_history(data->line);
		if (gSignalStatus == SIGINT)
		{
			free(data->line);
			continue ;
		}
		if (!lexing(data))
		{
			free_data(data);
			continue ;
		}
		t_token *tmp1 = data->tokens;
		while (tmp1)
		{
			if (tmp1->type == WORD)
				printf("'%s'\n", tmp1->word->txt);
			printf("%u\n", tmp1->type);
			tmp1 = tmp1->next;
		}
		if (!syntax_check(data))
		{
			printf("Syntax error !\n");
			free_data(data);
			continue ;
		}
		parsing(data);
		if (!data->cmd_list)
			exit_free(data, EXIT_FAILURE);
		print_env(data);
		t_cmd_list *tmp_list = data->cmd_list;
		while (tmp_list)
		{
			t_redirs *tmp_redirs = tmp_list->redirs;
			printf("\n//=== Command ===//\n");
			if (tmp_redirs)
			{
				printf("//=== Redirs ===//\n");
				while (tmp_redirs)
				{
					printf("file name : %s\n", tmp_redirs->file_name);
					tmp_redirs = tmp_redirs->next;
				}
			}
			t_word **tmp_args = tmp_list->args;
			while (*tmp_args)
			{
				printf("//=== Arguments ===//\n");
				printf("%s\n", (*tmp_args)->txt);
				printf("//=== Quoting ===//\n");
				int	i = 0;
				while ((*tmp_args)->txt[i])
					printf("%d ", (*tmp_args)->quoting[i++]);
				printf("\n");
				tmp_args++;
			}
			tmp_list = tmp_list->next;
		}
		expansion(data);
		t_expanded_list	*tmp = data->expanded_list;
		while (tmp)
		{
			char	**tmp_expanded_args = tmp->args;
			while (*tmp_expanded_args)
			{
				printf("%s\n", *tmp_expanded_args);
				tmp_expanded_args++;
			}
			tmp = tmp->next;
		}
		print_env(data);
		if (!ft_strncmp(*(data->expanded_list->args), "export", 6))
			ft_export(data, data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "echo", 4))
			ft_echo(data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "env", 3))
			print_env(data);
		else if (!ft_strncmp(*(data->expanded_list->args), "unset", 5))
			exec_unset(data, data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "cd", 2))
			exec_cd(data, data->expanded_list->args);
		prev_fd = -1;
		list = data->expanded_list;
		// si !list->next et commande = built_in, on ne rentre pas dans la boucle
		while (list)
		{
			data->last_pid = pipe_creator(data, &prev_fd, list);
			list = list->next;
		}
		wait_and_return(data);
	}
	free_env(&data->env);
	free(data->exit_status);
	free(data);
	rl_clear_history();
	return (0);
}*/
