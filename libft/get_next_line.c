/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:31:23 by nmichaud          #+#    #+#             */
/*   Updated: 2026/01/08 14:33:27 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_free(char **s1, char **s2)
{
	if (*s1 != NULL)
	{
		free(*s1);
		*s1 = NULL;
	}
	if (s2 != NULL && *s2 != NULL)
	{
		free(*s2);
		*s2 = NULL;
	}
	return (NULL);
}

static char	*ft_temp(char **tmp, char *buf)
{
	char	*res;

	if (*tmp == NULL)
	{
		res = ft_strdup(buf);
		if (!res)
			return (NULL);
	}
	else
	{
		res = ft_strjoin(*tmp, buf);
		if (!res)
			return (ft_free(tmp, NULL));
		free(*tmp);
	}
	return (res);
}

static	char	*ft_return_line(char **tmp)
{
	int		i;
	char	*res;
	char	*leftover;

	i = 0;
	while ((*tmp)[i] && (*tmp)[i] != '\n')
		i++;
	res = ft_substr(*tmp, 0, i + ((*tmp)[i] == '\n'));
	if (!res)
		return (ft_free(tmp, NULL));
	if (res[0] == '\0' && (*tmp)[i] == '\0')
		return (ft_free(tmp, &res));
	if ((*tmp)[i] == '\n')
		i++;
	leftover = NULL;
	if ((*tmp)[i] != '\0')
	{
		leftover = ft_strdup(&(*tmp)[i]);
		if (!leftover)
			return (ft_free(tmp, &res));
	}
	free(*tmp);
	*tmp = leftover;
	return (res);
}

static char	*loop(int fd, char **tmp, ssize_t s, char *buf)
{
	while (s > 0)
	{
		if (*tmp && ft_strchr(*tmp, '\n'))
			break ;
		buf = (char *)malloc(BUFFER_SIZE + 1);
		if (!buf)
			return (ft_free(tmp, NULL));
		s = read(fd, buf, BUFFER_SIZE);
		if (s <= 0)
		{
			free(buf);
			if (s < 0)
				ft_free(tmp, NULL);
			break ;
		}
		buf[s] = 0;
		*tmp = ft_temp(tmp, buf);
		free(buf);
		if (!(*tmp))
			return (NULL);
	}
	if (*tmp != NULL && **tmp != 0)
		return (ft_return_line(tmp));
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*tmp[1024];
	ssize_t		s;
	char		*buf;

	s = 1;
	buf = NULL;
	if (BUFFER_SIZE <= 0 || fd < 0 || fd >= 1024)
		return (NULL);
	return (loop(fd, &tmp[fd], s, buf));
}
