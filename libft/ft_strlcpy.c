/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:22:35 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/07 18:01:58 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t siz)
{
	size_t	i;

	if (siz == 0)
		return (ft_strlen(src));
	i = 0;
	while (i < siz - 1 && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}

/*#include <stdio.h>
#include <bsd/string.h>

int	main(int argc, char **argv)
{
	char	dest1[150];
	char	dest2[150];
	size_t	a;
	size_t	b;

	a = ft_strlcpy(dest1, argv[1], 0);
	b = strlcpy(dest2, argv[1], 0);
	printf("%s\n", dest1);
	printf("%s\n", dest2);
	printf("%zu\n", a);
	printf("%zu\n", b);
	return (0);
}*/	
