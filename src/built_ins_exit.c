/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:22:55 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/25 11:22:56 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_exit(t_data *data, char **args)
{
	int		i;

	ft_putstr_fd("exit\n", 2);
	if (!*(args + 1))
		exit_free(data, 0);
	i = 0;
	while (args[1][i])
	{
		if (!(args[1][i] <= '9' && args[1][i] >= '0'))
		{
			f_printf_2("minishell: exit: ", args[1],
				": numeric argument required\n");
			exit_free(data, 2);
		}
		i++;
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	i = ft_atoi(args[1]);
	exit_free(data, i);
	return (0);
}
