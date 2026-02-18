/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 09:06:18 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/13 19:56:42 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	str_size(long nb)
{
	int	result;

	result = 1;
	if (nb < 0)
	{
		nb = -nb;
		result++;
	}
	while (nb >= 10)
	{
		nb = nb / 10;
		result++;
	}
	return (result);
}	

static	int	negative(int c, long *nb, char *result)
{
	long	i;

	i = *nb;
	i = -i;
	*nb = i;
	result[0] = 45;
	c++;
	return (c);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		len;
	int		i;
	int		c;
	long	nb;

	c = 0;
	nb = (long)n;
	len = str_size(nb);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	if (nb < 0)
		c = negative(c, &nb, result);
	i = 0;
	while (i < len - c)
	{
		result[len - 1 - i] = nb % 10 + 48;
		nb = nb / 10;
		i++;
	}
	result[len] = 0;
	return (result);
}
