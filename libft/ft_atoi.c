/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:52:15 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/13 14:16:43 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;
	int	c;

	c = 0;
	result = 0;
	i = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == 45 || nptr[i] == 43)
	{
		i++;
		if (nptr[i - 1] == 45)
			c = 1;
	}
	while (nptr[i] < 58 && nptr[i] > 47)
	{
		result = result * 10 + nptr[i] - 48;
		i++;
	}
	if (c == 1)
		result = -result;
	return (result);
}
