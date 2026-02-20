/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dquote.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 18:51:28 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/17 18:51:34 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dquote(t_token **head, char *str, char c)
{
	char	*res;
	char	*old;
	char	*tmp;
	char	*line;
	int		i;
	int		flag;

	res = NULL;
	flag = 0;
	while (1)
	{
		line = readline("dquote> ");
		if (!line)
		{
			free_token(head);
			exit(EXIT_FAILURE);
		}
		i = 0;
		while (line[i])
		{
			if (line[i] == c)
				flag = 1;
			i++;
		}
		if (!res)
			tmp = str;
		else
			tmp = res;
		old = res;
		res = ft_strjoin(tmp, line);
		free(old);
		if (!res)
		{
			free(line);
			free_token(head);
			exit(EXIT_FAILURE);
		}
		free(line);
		if (flag)
			break ;
	}
	return (res);
}
