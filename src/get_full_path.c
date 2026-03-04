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

static char	*get_full_path(char *cmd, char **path)
{
	char	*tmp;
	char	*full_path;
	int		i;
	int		permission_flag;
	char	*tmp2;

	i = 0;
	permission_flag = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0)
				return (full_path);
			else
			{
				permission_flag = 1;
				tmp2 = ft_strdup(full_path);
				if (!tmp2)
					return (NULL);
			}
		}
		free(full_path);
		i++;
	}
	if (permission_flag == 1)
		return (tmp2);
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

/*static char	*cmd_slash(t_data *data, char *cmd, int *err)
{
	char	*res;
	struct	stat st;

	if (access(cmd, F_OK) == -1)
		*err = 123;
	else if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		*err = 124;
	else if (access(cmd, X_OK) == -1)
		*err = 125;
	else
	{
		res = ft_strdup(cmd);
		if (!res)
			exit_free(data, EXIT_FAILURE);
		return (res);
	}
	return (NULL);
}

static char	*get_full_path(char *cmd, char **path, int *err)
{
	char	*tmp;
	char	*full_path;
	int		i;
	int		permission_flag;

	i = 0;
	permission_flag = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0)
				return (full_path);
			else
				permission_flag = 1;
		}
		free(full_path);
		i++;
	}
	if (permission_flag == 0)
		*err = 126;
	else
		*err = 127;
	return (NULL);
}

char	*find_cmd(t_data *data, char *cmd, char **path, int *err)
{
	int		i;
	char	*full_path;

	if (!cmd || cmd[0] == '\0')
	{
		*err = 126;
		return (NULL);
	}
	if (ft_strchr(cmd, '/') != NULL)
		return (cmd_slash(data, cmd, err));
	i = 0;
	full_path = get_full_path(cmd, path, err);
	return (full_path);
}*/
