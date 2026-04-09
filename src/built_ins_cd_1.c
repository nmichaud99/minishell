/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_cd_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:41:52 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 15:41:56 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_pwd_line(t_data *data, char *var_name)
{
	char	*pwd_join;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		return (NULL);
	}
	pwd_join = ft_strjoin(var_name, pwd);
	free(pwd);
	if (!pwd_join)
		exit_free(data, EXIT_FAILURE);
	return (pwd_join);
}

int	count_args_nb(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	handle_no_arg(t_data *data, char **new_directory, int *return_value)
{
	if (!find_key(data, "HOME"))
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		*return_value = 1;
		return ;
	}
	*new_directory = get_variable_value(data, "HOME");
	if (!*new_directory)
	{
		*return_value = 1;
		return ;
	}
	if (ft_strlen(*new_directory) == 0)
	{
		free(*new_directory);
		*return_value = 0;
		return ;
	}
	*return_value = 2;
}
