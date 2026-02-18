/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:22:23 by nmichaud          #+#    #+#             */
/*   Updated: 2025/12/18 15:35:32 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_integer(int n, const char **format)
{
	int		res;
	char	*str;

	str = ft_itoa(n);
	ft_putstr_fd(str, 1);
	res = ft_strlen(str);
	free(str);
	(*format)++;
	return (res);
}

int	ft_pointer(unsigned long n, const char **format)
{
	int	res;

	if (n == 0)
	{
		ft_putstr_fd("(nil)", 1);
		res = 5;
	}
	else
	{
		ft_putstr_fd("0x", 1);
		res = ft_putnbr_base(n, "0123456789abcdef") + 2;
	}
	(*format)++;
	return (res);
}

int	ft_string(char *str, const char **format)
{
	int	res;

	res = 6;
	if (str == NULL)
		ft_putstr_fd("(null)", 1);
	else
	{
		ft_putstr_fd(str, 1);
		res = ft_strlen(str);
	}
	(*format)++;
	return (res);
}

int	ft_char(int i, const char **format)
{
	int		res;
	char	c;

	res = 1;
	c = (char)i;
	ft_putchar_fd(c, 1);
	(*format)++;
	return (res);
}

int	ft_unsigned_int(unsigned int u, const char **format)
{
	int		res;
	char	*str;

	str = ft_itoa_unsigned(u);
	ft_putstr_fd(str, 1);
	res = ft_strlen(str);
	free(str);
	(*format)++;
	return (res);
}
