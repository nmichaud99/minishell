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

static char	*cmd_slash(t_data *data, char *cmd, int *err)
{
	char	*res;

	if (access(cmd, F_OK) == -1)
		*err = 127;
	else if (access(cmd, X_OK) == -1)
		*err = 126;
	else
	{
		res = ft_strdup(cmd);
		if (!res)
			exit_free(data, EXIT_FAILURE);
		return (res);
	}
	return (NULL);
}

static char	*get_full_path(char *cmd, char **path, int *flag)
{
	char	*tmp;
	char	*full_path;
	int		i;

	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
			return (full_path);
		else if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == -1)
			*flag = 1;
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_cmd(t_data *data, char *cmd, char **path, int *err)
{
	int		i;
	char	*full_path;
	int		flag;

	flag = 0;
	if (!cmd || cmd[0] == '\0')
	{
		*err = 126;
		return (NULL);
	}
	if (ft_strchr(cmd, '/') != NULL)
		return (cmd_slash(data, cmd, err));
	i = 0;
	full_path = get_full_path(cmd, path, &flag);
	if (full_path != NULL)
		return (full_path);
	if (flag == 1)
		*err = 126;
	else
		*err = 127;
	return (NULL);
}
