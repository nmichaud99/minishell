/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:11:53 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 13:11:55 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_free(t_data *data, int status)
{
	if (!data)
		exit(status);
	free_data(data);
	free_expanded_list(&data->expanded_list);
	free_env(&data->env);
	ft_free(&data->env_tab);
	exit(status);
}

void	error_sys(t_data *data, char *s)
{
	perror(s);
	exit_free(data, data->exit_status);
}
