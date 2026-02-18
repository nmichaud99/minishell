/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_unsigned.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 09:06:18 by nmichaud          #+#    #+#             */
/*   Updated: 2025/12/18 15:35:05 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	str_size(unsigned int nb)
{
	int	result;

	result = 1;
	while (nb >= 10)
	{
		nb = nb / 10;
		result++;
	}
	return (result);
}	

char	*ft_itoa_unsigned(unsigned int nb)
{
	char	*result;
	int		len;
	int		i;
	int		c;

	c = 0;
	len = str_size(nb);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
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
