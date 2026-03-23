/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 22:03:15 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/03 22:03:26 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pwd(t_data *data, char **args)
{
	char	*pwd;

	(void)data;
	(void)args;
	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
}

static char	*build_pwd_line(t_data *data, char *var_name)
{
	char	*pwd_join;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	pwd_join = ft_strjoin(var_name, pwd);
	free(pwd);
	if (!pwd_join)
		exit_free(data, EXIT_FAILURE);
	return (pwd_join);
}

int	exec_cd(t_data *data, char **args)
{
	char	*old_pwd_line;
	char	*new_pwd_line;

	if (args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	old_pwd_line = build_pwd_line(data, "OLDPWD=");
	add_or_modify_env_node(data, old_pwd_line);
	free(old_pwd_line);
	if (*(args + 1) && chdir(*(args + 1)) != 0)
	{
		ft_perror(args);
		return (1);
	}
	new_pwd_line = build_pwd_line(data, "PWD=");
	add_or_modify_env_node(data, new_pwd_line);
	free(new_pwd_line);
	return (0);
}
