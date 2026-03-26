/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_printf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 09:13:26 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/05 09:13:41 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_perror(char **arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg[0], 2);
	ft_putstr_fd(": ", 2);
	if (arg[1])
	{
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

void	ft_perror_2(char *s1, char *s2, char *s3)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(s1, 2);
	if (s2 && ft_strlen(s2) > 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s2, 2);
	}
	if (s3 && ft_strlen(s3) > 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s3, 2);
	}
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

void	f_printf(char *s1, char *s2)
{
	int	size1;
	int	size2;

	write(2, "minishell: ", 11);
	size1 = ft_strlen(s1);
	write(2, s1, size1);
	if (s2)
	{
		size2 = ft_strlen(s2);
		write(2, ": ", 2);
		write(2, s2, size2);
	}
}

void	f_printf_2(char *s1, char *s2, char *s3)
{
	int	size1;
	int	size2;
	int	size3;

	if (!s1 || !s2 || !s3)
		return ;
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	size3 = ft_strlen(s3);
	write(2, s1, size1);
	write(2, s2, size2);
	write(2, s3, size3);
}

void	f_printf_3(char *s1, char *s2, char *s3)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(s1, 2);
	if (s2 && ft_strlen(s2) > 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s2, 2);
	}
	if (s3 && ft_strlen(s3) > 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s3, 2);
	}
	ft_putstr_fd("\n", 2);
}
