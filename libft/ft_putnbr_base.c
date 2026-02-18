/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:27:09 by nmichaud          #+#    #+#             */
/*   Updated: 2025/12/18 15:35:59 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr_base_rec(long nbr, char *base, int *i)
{
	unsigned long	l;
	unsigned long	a;

	a = nbr;
	l = ft_strlen(base);
	if (a < 0)
	{
		ft_putchar('-');
		a = -a;
		(*i)++;
	}
	if (a < l)
	{
		ft_putchar(base[a]);
		(*i)++;
	}
	else
	{
		ft_putnbr_base_rec(a / l, base, i);
		ft_putnbr_base_rec(a % l, base, i);
	}
}

int	ft_putnbr_base(long nbr, char *base)
{
	int	count;

	count = 0;
	ft_putnbr_base_rec(nbr, base, &count);
	return (count);
}

/*int	main(void)
{
	int	i;
	char	*base = "0123456789abcdef";

	i = -2147483648;
	ft_putnbr_base(i, base);
	return (0);
}*/
