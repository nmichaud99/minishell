/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:35:43 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/13 20:24:33 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	int		len_to_alloc;
	char	*result;

	len_to_alloc = len + 1;
	if (ft_strlen(s) - start < len)
		len_to_alloc = ft_strlen(s) - start + 1;
	if (start > ft_strlen(s) - 1 || s[0] == 0)
		len_to_alloc = 1;
	result = (char *)malloc(len_to_alloc);
	if (!result)
		return (NULL);
	if (len_to_alloc == 1)
	{
		result[0] = 0;
		return (result);
	}
	i = 0;
	while (i < len && s[start + i])
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = 0;
	return (result);
}
