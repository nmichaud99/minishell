/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 19:56:27 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/13 13:57:43 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	c1;

	c1 = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == c1)
			return ((char *)&s[i]);
		i++;
	}
	if (c1 == 0)
		return ((char *)&s[i]);
	return (NULL);
}
