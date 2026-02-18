/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:15:36 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/13 20:05:53 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	temp;

	if (little[0] == 0)
		return ((char *)big);
	i = 0;
	j = 0;
	while (i < len && big[i])
	{
		temp = i;
		while (little[j] == big[temp] && temp < len && big[temp])
		{
			temp++;
			j++;
		}
		if (j == ft_strlen(little) || little[j] == 0)
			return ((char *)&big[temp - j]);
		i++;
		j = 0;
	}
	return (NULL);
}
