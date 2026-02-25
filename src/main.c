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
	data->tokens = NULL;
	data->nodes = NULL;
	data->line = NULL;
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
	char		**args;
	//t_cmd_list	*list;

	(void)ac;
	(void)av;
	(void)env;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	init_data(data);
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
					printf("%s\n", *args);
					args++;
				}
				cmd_list = cmd_list->next;
			}
		}
		free_token(&data->tokens);
		free(data->line);
	}
	free(data);
	rl_clear_history();
	return (0);
}
