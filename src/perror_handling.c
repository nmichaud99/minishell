/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perror_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:40:16 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 16:40:17 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_perror(char **arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg[0], 2);
	ft_putstr_fd(": ", 2);
	if (arg[1])
	{
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}
