/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_export_3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:56:31 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 12:56:32 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_export(t_data *data, char **args)
{
	int	invalid_flag;

	invalid_flag = 0;
	if (!*(args + 1))
		return (print_env_export(data), 0);
	else
	{
		args++;
		if (!handle_args(args, &invalid_flag, data))
			return (1);
	}
	if (invalid_flag)
		return (1);
	return (0);
}
