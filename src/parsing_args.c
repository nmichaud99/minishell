/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:52:33 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 11:52:34 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *start, t_token *end)
{
	t_token	*tmp;
	t_token	*prev;
	int		args_size;

	tmp = start;
	args_size = 0;
	prev = NULL;
	while (tmp != end)
	{
		if (tmp->type == WORD)
		{
			if (prev && is_redir(prev->type))
			{
				prev = tmp;
				tmp = tmp->next;
				continue ;
			}
			args_size++;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (args_size);
}

t_word	*new_arg(t_token *tmp, int *flag)
{
	t_word	*ret;

	ret = malloc(sizeof(t_word));
	if (!ret)
	{
		*flag = 1;
		return (NULL);
	}
	ret->txt = ft_strdup(tmp->word->txt);
	if (!ret->txt)
	{
		*flag = 1;
		free(ret);
		return (NULL);
	}
	ret->quoting = dup_quoting(tmp->word);
	if (!ret->quoting)
	{
		*flag = 1;
		free(ret->txt);
		free(ret);
		return (NULL);
	}
	return (ret);
}

static void	init_variables(t_token **tmp, t_token **prev,
							t_token *start, int *i)
{
	*tmp = start;
	*prev = NULL;
	*i = 0;
}

int	build_args(t_word **args, t_token *start, t_token *end, int *flag)
{
	t_token	*tmp;
	t_token	*prev;
	int		i;

	init_variables(&tmp, &prev, start, &i);
	while (tmp != end)
	{
		if (tmp->type == WORD && prev && is_redir(prev->type))
		{
			prev = tmp;
			tmp = tmp->next;
			continue ;
		}
		else if (tmp->type == WORD)
		{
			args[i] = new_arg(tmp, flag);
			if (!args[i])
				return (free_word_tab_2(&args, i), 0);
			i++;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

t_word	**get_args(t_token *start, t_token *end, int *flag)
{
	int			nb_args;
	t_word		**args;

	nb_args = count_args(start, end);
	args = malloc(sizeof(t_word *) * (nb_args + 1));
	if (!args)
	{
		*flag = 1;
		return (NULL);
	}
	null_init(args, nb_args + 1);
	if (!build_args(args, start, end, flag))
		return (NULL);
	return (args);
}
