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

int	long_check(char *str)
{
	if (ft_strlen(str) > 20)
		return (0);
	if (ft_strlen(str) == 20 && ft_strcmp(str, "-9223372036854775808") > 0)
		return (0);
	else if (str[0] && str[0] != '-'
		&& (ft_strlen(str) > 19 || (ft_strlen(str) == 19
				&& ft_strcmp(str, "9223372036854775807") > 0)))
		return (0);
	return (1);
}

int	is_valid(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long long	ft_atoi_exit(char *num)
{
	int			i;
	long		sign;
	long long	res;

	sign = 1;
	i = 0;
	res = 0;
	while (num[i] && (num[i] == ' ' || num[i] == '\n' || num[i] == '\t'
			|| num[i] == '\v' || num[i] == '\f' || num[i] == '\r'))
		i++;
	if (num[i] == '-' || num[i] == '+')
	{
		if (num[i] == '-')
			sign *= -1;
		i++;
	}
	while (num[i] && (num[i] >= '0' && num[i] <= '9'))
	{
		res = res * 10 + (num[i] - '0');
		i++;
	}
	return (res * sign);
}

void	num_required(t_data *data, char *arg)
{
	f_printf_2("minishell: exit: ", arg,
		": numeric argument required\n");
	close_saved_stds(data);
	exit_free(data, 2);
}

int	exec_exit(t_data *data, char **args)
{
	long long	i;

	ft_putstr_fd("exit\n", 2);
	if (!*(args + 1))
	{
		close_saved_stds(data);
		exit_free(data, data->exit_status);
	}
	i = 0;
	if (!is_valid(args[1]) || !long_check(args[1]))
		num_required(data, args[1]);
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	i = ft_atoi_exit(args[1]);
	close_saved_stds(data);
	exit_free(data, (unsigned char)i % 256);
	return (0);
}

/*int	exec_exit(t_data *data, char **args)
{
	int		i;

	ft_putstr_fd("exit\n", 2);
	if (!*(args + 1))
		exit_free(data, data->exit_status);
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
}*/
