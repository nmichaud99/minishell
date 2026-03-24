/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 21:16:57 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/03 21:17:08 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_args(int option_n, char **args)
{
	while (*args)
	{
		if (*(args + 1))
			printf("%s ", *args);
		else
		{
			printf("%s", *args);
			if (option_n)
				printf("\n");
		}
		args++;
	}
}

int	is_option(char *str)
{
	int	i;

	if (!str)
		return (0);
	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	exec_echo(char **args)
{
	char	**tmp_args;
	int		option;

	option = 1;
	tmp_args = args + 1;
	while (*tmp_args && is_option(*tmp_args))
	{
		option = 0;
		tmp_args++;
	}
	print_args(option, tmp_args);
	return (0);
}
