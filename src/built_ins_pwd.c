/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:58:36 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/02 18:58:37 by nmichaud         ###   ########.fr       */
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

/*
bash-5.1$ pwd test
/home/fjerrige/Documents/minishell

bash-5.1$ pwd echo test
/home/fjerrige/Documents/minishell

bash-5.1$ pwd echo test | grep test
bash-5.1$

bash-5.1$ pwd | echo test
test

bash-5.1$ pwd | grep /
/home/fjerrige/Documents/minishell

bash-5.1$ pwd | grep i
/home/fjerrige/Documents/minishell

bash-5.1$ pwd | grep z
bash-5.1$

*/
