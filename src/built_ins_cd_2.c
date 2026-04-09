/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_cd_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:42:00 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 15:42:05 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_old_pwd(t_data *data, char *new_directory)
{
	char	*old_pwd_line;

	old_pwd_line = build_pwd_line(data, "OLDPWD=");
	if (!old_pwd_line)
	{
		if (chdir(new_directory) != 0)
			ft_perror_2("cd", new_directory, NULL);
		else
		{
			ft_putstr_fd("chdir: error retrieving current directory: ", 2);
			ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
			ft_putstr_fd("No such file or directory\n", 2);
		}
		return (free(new_directory), 0);
	}
	add_or_modify_env_node(data, old_pwd_line, 1);
	free(old_pwd_line);
	return (1);
}

int	handle_new_pwd(t_data *data, char *new_directory)
{
	char	*new_pwd_line;

	new_pwd_line = build_pwd_line(data, "PWD=");
	if (!new_pwd_line)
		return (free(new_directory), 0);
	add_or_modify_env_node(data, new_pwd_line, 1);
	return (free(new_directory), free(new_pwd_line), 1);
}

int	ft_chdir(t_data *data, char *new_directory)
{
	if (!handle_old_pwd(data, new_directory))
		return (0);
	if (chdir(new_directory) != 0)
	{
		ft_perror_2("cd", new_directory, NULL);
		return (free(new_directory), 0);
	}
	if (!handle_new_pwd(data, new_directory))
		return (0);
	return (1);
}

int	exec_cd(t_data *data, char **args)
{
	char	*new_directory;
	int		return_value;
	int		len;

	return_value = 0;
	len = count_args_nb(args);
	if (len >= 3)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	if (len == 1)
	{
		handle_no_arg(data, &new_directory, &return_value);
		if (return_value != 2)
			return (return_value);
	}
	else
	{
		new_directory = ft_strdup(*(args + 1));
		if (!new_directory)
			return (1);
	}
	if (!ft_chdir(data, new_directory))
		return (1);
	return (0);
}
