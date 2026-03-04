/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 21:16:57 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/03 21:17:08 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	char	**tmp_args;

	if (*(args + 1) && !ft_strncmp(*(args + 1), "-n", 2))
	{
		tmp_args = args + 2;
		while (*tmp_args)
		{
			if (*(tmp_args + 1))
				printf("%s ", *tmp_args);
			else
				printf("%s", *tmp_args);
			tmp_args++;
		}
		return (0);
	}
	else
	{
		tmp_args = args + 1;
		while (*tmp_args)
		{
			if (*(tmp_args + 1))
				printf("%s ", *tmp_args);
			else
				printf("%s\n", *tmp_args);
			tmp_args++;
		}
	}
	return (0);
}
