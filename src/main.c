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

void	init_data(t_data *data)
{
	data->tokens = malloc(sizeof(t_token));
	if (!data->tokens)
		exit_free(data, EXIT_FAILURE);
	data->line = NULL;
}

void	sigint_handler(sig)
int sig;
{
	printf("%d received\n",sig);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	(void)ac;
	(void)av;
	(void)env;
	signal(SIGINT, sigint_handler);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	init_data(data);
	while (1)
	{
		data->line = readline("minishell$ ");
		if (!data->line)
			break ;
		if (*(data->line))
			add_history(data->line);
		lexing(data);
		t_token *tmp = *(data->tokens);
		while (tmp)
		{
			printf("'%s'\n", tmp->str);
			printf("%u\n", tmp->type);
			tmp = tmp->next;
		}
		free_token(data->tokens);
		free(data->line);
	}
	free(data);
	rl_clear_history();
	return (0);
}
