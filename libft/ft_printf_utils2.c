/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:45:33 by nmichaud          #+#    #+#             */
/*   Updated: 2025/12/18 15:35:43 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hexa(unsigned int u, const char **format)
{
	int	res;

	if (**format == 'x')
		res = ft_putnbr_base(u, "0123456789abcdef");
	else
		res = ft_putnbr_base(u, "0123456789ABCDEF");
	(*format)++;
	return (res);
}
