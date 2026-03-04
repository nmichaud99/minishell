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
	//char	*new_pwd_arg;
	//char	*old_pwd_arg;

	// get old pwd before cd
	old_pwd = getcwd(NULL, 0);

	// build line for add or modify node
	/*
	new_pwd_arg = ft_strjoin("PWD=", );
	if (!new_pwd_arg)
		return (NULL);
	*/



	printf("Old Pwd : %s\n", old_pwd);

	// exec cd
	if (chdir(*(args + 1)) != 0)
	{
		printf("cd: %s: No such file or directory\n", *(args + 1));
		return (1);
	}

	// update env : PWD = getcwd
	add_or_modify_env_node(data, "PWD=test");
	new_pwd = getcwd(NULL, 0);
	add_or_modify_env_node(data, "PWD=test");
	printf("New Pwd : %s\n", new_pwd);
	printf("cd: SUCCESS!\n");
	return (0);
}
