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

void	init_data(t_data *data)
{
	data->line = NULL;
	data->tokens = NULL;
	data->cmd_list = NULL;
	data->env = NULL;
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
	while (1)
	{
		init_data(data);
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
		if (!syntax_check(data))
		{
			printf("Syntax error !\n");
			free_data(data);
			continue ;
		}
		parsing(data);
		if (!data->cmd_list)
			exit_free(data, EXIT_FAILURE);
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
			char **tmp_args = tmp_list->args;
			if (*tmp_args)
			{
				printf("//=== Arguments ===//\n");
				while (*tmp_args)
				{
					printf("%s\n", *tmp_args);
					tmp_args++;
				}
			}
			tmp_list = tmp_list->next;
		}
		free_token(&data->tokens);
		free_list(&data->cmd_list);
		free(data->line);
	}
	free(data);
	rl_clear_history();
	return (0);
}
