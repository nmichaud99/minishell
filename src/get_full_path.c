/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_full_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:54:42 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/04 15:54:52 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_path(char *cmd, char *path)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full_path)
		return (NULL);
	return (full_path);
}

int	access_path(char *path, int *permission_flag, char **tmp)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (1);
		else
		{
			*permission_flag = 1;
			*tmp = ft_strdup(path);
			if (!*tmp)
				return (-1);
		}
	}
	return (0);
}

char	*get_full_path(char *cmd, char **path)
{
	char	*full_path;
	int		i;
	int		permission_flag;
	char	*tmp;

	i = 0;
	permission_flag = 0;
	while (path[i])
	{
		full_path = build_path(cmd, path[i]);
		if (!full_path)
			return (NULL);
		if (access_path(full_path, &permission_flag, &tmp) == 1)
			return (full_path);
		if (access_path(full_path, &permission_flag, &tmp) == -1)
			return (NULL);
		free(full_path);
		i++;
	}
	if (permission_flag == 1)
		return (tmp);
	return (NULL);
}

char	*find_cmd(char *cmd, char **path)
{
	char	*full_path;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL)
	{
		full_path = ft_strdup(cmd);
		if (!full_path)
			return (NULL);
		return (full_path);
	}
	full_path = get_full_path(cmd, path);
	return (full_path);
}
