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
		data->exit_status = malloc(sizeof(int *));
		if (!data->exit_status)
			exit(EXIT_FAILURE);
		*(data->exit_status) = 0;
	}
	else
	{
		free_token(&data->tokens);
		free_list(&data->cmd_list);
		free_expanded_list(&data->expanded_list);
		free(data->line);
		data->line = NULL;
	}
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
	t_data		*data;

	(void)ac;
	(void)av;
	(void)env;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->env = NULL;
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
		/*t_token *tmp1 = data->tokens;
		while (tmp1)
		{
			if (tmp1->type == WORD)
				printf("'%s'\n", tmp1->word->txt);
			printf("%u\n", tmp1->type);
			tmp1 = tmp1->next;
		}*/
		if (!syntax_check(data))
		{
			printf("Syntax error !\n");
			free_data(data);
			continue ;
		}
		parsing(data);
		if (!data->cmd_list)
			exit_free(data, EXIT_FAILURE);
		/*print_env(data);
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
		}*/
		expansion(data);
		/*t_expanded_list	*tmp = data->expanded_list;
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
		print_env(data);*/
		if (!ft_strncmp(*(data->expanded_list->args), "export", 6))
			ft_export(data, data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "echo", 4))
			ft_echo(data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "env", 3))
			exec_env(data, data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "unset", 5))
			exec_unset(data, data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "cd", 2))
			exec_cd(data, data->expanded_list->args);
		else if (!ft_strncmp(*(data->expanded_list->args), "pwd", 3))
			exec_pwd(data, data->expanded_list->args);
	}
	free_env(&data->env);
	free(data);
	rl_clear_history();
	return (0);
}

/*int	main(int ac, char **av, char **env)
{
	t_data		*data;
	char		**args;
	//t_cmd_list	*list;

	(void)ac;
	(void)av;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	init_data(data);
	init_env_tab(env, data);
	print_env(data);
	while (1)
	{
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
		lexing(data);
		t_token *tmp = data->tokens;
		while (tmp)
		{
			printf("'%s'\n", tmp->str);
			printf("%u\n", tmp->type);
			tmp = tmp->next;
		}
		t_cmd_list *cmd_list = use_tokens(&data->tokens);
		while (cmd_list)
		{
			printf("//=== Command ===//\n");
			if (cmd_list->redirs && cmd_list->redirs->file_name)
				printf("file name : %s\n", cmd_list->redirs->file_name);
			if (cmd_list->args)
				args = cmd_list->args;
			if (*args)
			{
				while (*args)
				{
					if (ft_strlen(*args) == 6 && ft_strcmp(*args, "export") == 0)
					{
						printf("arg after export : %s\n", *(args + 1));
						add_or_modify_env_node(data, *(args + 1));
					}
					if (ft_strlen(*args) == 5 && ft_strcmp(*args, "unset") == 0)
					{
						printf("arg after unset : %s\n", *(args + 1));
						exec_unset(data, *(args + 1));
					}
					printf("%s\n", *args);
					args++;
				}
				cmd_list = cmd_list->next;
			}
		}
		free_token(&data->tokens);
		free(data->line);
	}
	print_env(data);
	free(data);
	rl_clear_history();
	return (0);
}*/
