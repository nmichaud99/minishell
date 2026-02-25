/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:11:50 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 15:11:50 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_env_tab(char **env)
{
	char	**env_tab;
	int		i;

	i = 0;
	while (env[i])
		i++;
	env_tab = malloc(sizeof(char *) * (i + 1));
	if (!env_tab)
		return (NULL);
	i = 0;
	while (env[i])
	{
		env_tab[i] = ft_strdup(env[i]);
		i++;
	}
	env_tab[i] = NULL;
	i = 0;
	while (env_tab[i])
	{
		//printf("env : %s\n", env_tab[i]);
		i++;
	}
	return (env_tab);
}


// Add Variable

// Modify Variable
