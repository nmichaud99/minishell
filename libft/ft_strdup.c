/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:08:39 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/06 17:40:54 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*result;
	int		i;

	result = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (s[i])
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

/*#include <stdio.h>

int	main(void)
{
	char	*str = "string a dup";
	char	*dup;
	char	*temoin;

	dup = ft_strdup(str);
	temoin = strdup(str);
	printf("%s\n", dup);
	printf("%s\n", temoin);
	free(dup);
	free(temoin);
	return (0);
}*/
