/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:03:34 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/02 19:03:37 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
- recuperer pwd
- mettre a jour env avec pwd et oldpwd a chque deplacement
*/

int	exec_cd(t_data *data, char **args)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	printf("Old Pwd : %s\n", old_pwd);
	if (chdir(*(args + 1)) != 0)
	{
		printf("cd: %s: No such file or directory\n", *(args + 1));
		return (1);
	}
	add_or_modify_env_node(data, "PWD=test");
	new_pwd = getcwd(NULL, 0);
	printf("New Pwd : %s\n", new_pwd);
	printf("cd: SUCCESS!\n");
	return (0);
}
