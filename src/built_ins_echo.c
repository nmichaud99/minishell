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

int	ft_putstr_fd_safe(char *s, int fd)
{
	int	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (write(fd, s, len) == -1)
	{
		if (errno == EPIPE)
			exit(0);
		return (-1);
	}
	return (0);
}

void	print_args(int option_n, char **args, int fd)
{
	while (*args)
	{
		if (*(args + 1))
			ft_putstr_fd_safe(*args, fd);
		else
		{
			ft_putstr_fd_safe(*args, fd);
			if (option_n)
				ft_putstr_fd_safe("\n", fd);
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

int	exec_echo(char **args, int fd)
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
	print_args(option, tmp_args, fd);
	return (0);
}
