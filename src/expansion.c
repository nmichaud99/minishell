/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:03:19 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/02 19:03:23 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_data *data)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp && tmp->next)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	printf("%s=%s\n", tmp->key, tmp->value);
}

char	*get_variable_value(t_data *data, char *str)
{
	t_env	*tmp;
	char	*tmp_value;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, str) == 0)
		{
			tmp_value = ft_strdup(tmp->value);
			if (!tmp_value)
				exit_free(data, EXIT_FAILURE);
			return (tmp_value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	handle_variable(t_token **head, char *str, int *i)
{
	char 	*res;
	int		start;

	(*i)++;
	start = *i;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '\'' || str[*i] == '"'
		|| is_operator(str[*i]))
	{
		res = malloc(2);
		if (!res)
			exit(EXIT_FAILURE);
		res[0] = '$';
		res[1] = 0;
		add_token(head, new_token(WORD, res));
		return ;
	}
	while (str[*i] && str[*i] != ' ' && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	res = malloc(*i - start + 1);
	if (!res)
		exit(EXIT_FAILURE);
	ft_strlcpy(res, str + start, *i - start + 1);
	add_token(head, new_token(VARIABLE, res));
}
