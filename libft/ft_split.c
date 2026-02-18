/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:11:18 by nmichaud          #+#    #+#             */
/*   Updated: 2025/11/17 13:16:18 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	count_c(char const *s, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			if (s[i + 1] == c || !s[i + 1])
			{
				count++;
				i++;
			}
			else
				i++;
		}
		else
			i++;
	}
	return (count);
}

static	char	*ft_sep(char **res, char const *s, size_t start, size_t end)
{
	size_t		i;
	char		*result;

	result = (char *)malloc(end - start + 1);
	if (!result)
	{
		i = 0;
		while (res[i])
			free(res[i++]);
		free(res);
		return (NULL);
	}
	i = 0;
	while (i < end - start)
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = 0;
	return (result);
}

static	int	action(char const *s, char c, char **result)
{
	size_t	i;
	size_t	j;
	size_t	start;
	size_t	end;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c && s[i])
		{
			start = i;
			while (s[i] != c && s[i])
				i++;
			end = i;
			result[j] = ft_sep(result, s, start, end);
			if (!result[j])
				return (0);
			result[j + 1] = NULL;
			j++;
		}
		else
			i++;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (count_c(s, c) + 1));
	if (!result)
		return (NULL);
	result[0] = NULL;
	if (action(s, c, result) == 0)
		return (NULL);
	return (result);
}
