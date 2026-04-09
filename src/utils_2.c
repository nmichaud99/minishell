/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:07:23 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 16:07:24 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_saved_stds(t_data *data)
{
	if (data->saved_stdin != -1)
		close(data->saved_stdin);
	if (data->saved_stdout != -1)
		close(data->saved_stdout);
}

void	dup_and_close(t_data *data, int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		close(fd1);
		error_sys(data, "dup2 error 2");
	}
	close(fd1);
}

int	ft_schr(const char *s, char c)
{
	int		i;
	char	c1;

	c1 = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == c1)
			return (1);
		i++;
	}
	return (0);
}

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_alpha(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
