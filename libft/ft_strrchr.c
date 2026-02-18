/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 08:57:21 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/13 13:59:28 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	c1;

	c1 = (char)c;
	i = ft_strlen(s) - 1;
	if (c1 == 0)
		return ((char *)&s[i + 1]);
	while (i >= 0)
	{
		if (s[i] == c1)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
