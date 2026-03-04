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

int	exec_cd(t_data *data, char **args)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*old_pwd_join;
	char	*new_pwd_join;

	//print_env(data);
	if (args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	old_pwd = getcwd(NULL, 0);
	old_pwd_join = ft_strjoin("OLDPWD=", old_pwd);
	free(old_pwd);
	if (!old_pwd_join)
	{
		free(old_pwd_join);
		exit_free(data, EXIT_FAILURE);
	}
	printf("Old Pwd Join : %s\n", old_pwd_join);
	add_or_modify_env_node(data, old_pwd_join);
	free(old_pwd_join);
	if (*(args + 1) && chdir(*(args + 1)) != 0)
	{
		ft_perror(args);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	new_pwd_join = ft_strjoin("PWD=", new_pwd);
	free(new_pwd);
	if (!new_pwd_join)
	{
		free(new_pwd_join);
		exit_free(data, EXIT_FAILURE);
	}
	printf("New Pwd Join : %s\n", new_pwd_join);
	add_or_modify_env_node(data, new_pwd_join);
	free(new_pwd_join);
	printf("cd: SUCCESS!\n");
	//print_env(data);
	return (0);
}
