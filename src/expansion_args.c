/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:07:36 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 12:07:46 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			str[i] = 29;
		i++;
	}
}

char	*expand_word(t_data *data, t_word *arg, int *i, t_quote_type quote)
{
	char	*tmp;
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	if (quote == SPECIAL)
	{
		(*i) = start;
		res = ft_strdup("");
		if (!res)
			return (NULL);
		return (res);
	}
	if (quote == SPECIAL && !arg->txt[start])
	{
		(*i) = start + 1;
		res = ft_strdup("$");
		if (!res)
			return (NULL);
		return (res);
	}
	if (arg->txt[start] && arg->quoting[start] != quote && quote != SPECIAL)
	{
		(*i) = start;
		res = ft_strdup("");
		if (!res)
			return (NULL);
		return (res);
	}
	if (arg->txt[start] == '?')
	{
		(*i) = start + 1;
		tmp = "?";
		res = get_variable_value(data, tmp);
		if (!res)
			return (NULL);
		return (res);
	}
	if (arg->txt[start] && (arg->txt[start] <= '9' && arg->txt[start] >= '0'))
	{
		(*i) = start + 1;
		res = ft_strdup("");
		if (!res)
			return (NULL);
		return (res);
	}
	if (!(arg->txt[start] && (type_of_char(arg->txt[start],
			arg->quoting[start], quote) == 1)))
	{
		(*i)++;
		res = ft_strdup("$");
		if (!res)
			return (NULL);
		return (res);
	}
	count = 0;
	while (arg->txt[start + count] && (type_of_char(arg->txt[start + count],
			arg->quoting[start + count], quote) >= 1))
		count++;
	tmp = malloc(count + 1);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, &arg->txt[start], count);
	tmp[count] = 0;
	res = get_variable_value(data, tmp);
	free(tmp);
	if (!res)
		return (NULL);
	if (quote == NONE)
		replace_spaces(res);
	*i = start + count;
	return (res);
}

char	**expand_arg(t_data *data, t_word *arg)
{
	char			**res;
	char			*tmp;
	char			*variable;
	int				i;

	i = 0;
	tmp = ft_strdup("");
	if (!tmp)
		return (NULL);
	while (arg->txt[i])
	{
		if (arg->txt[i] == '$' && arg->quoting[i] != SINGLE)
		{
			variable = expand_word(data, arg, &i, arg->quoting[i]);
			if (!variable || !append_variable(&tmp, &variable))
				return (free(tmp), NULL);
		}
		else
		{
			if (!append_char(&tmp, arg->txt[i]))
				return (free(tmp), NULL);
			i++;
		}
	}
	res = ft_split(tmp, 29);
	free(tmp);
	return (res);
}

char	**dup_args(char **str)
{
	int		i;
	char	**res;

	i = 0;
	while (str[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
	{
		res[i] = ft_strdup(str[i]);
		if (!res[i])
			return (ft_free(&res), NULL);
		i++;
	}
	res[i] = NULL;
	return (res);
}

int	add_arg_node(t_arg_list **head, char **str)
{
	t_arg_list	*tmp;
	t_arg_list	*new;
	char		**args;

	new = malloc(sizeof(t_arg_list));
	if (!new)
		return (0);
	args = dup_args(str);
	if (!args)
		return (free(new), 0);
	new->args = args;
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return (1);
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}

char	**build_expanded_args(t_arg_list **head)
{
	char		**res;
	int			i;
	int			j;
	int			count;
	t_arg_list	*tmp;

	count = 0;
	tmp = *head;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
			i++;
		count += i;
		tmp = tmp->next;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	j = 0;
	tmp = *head;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			res[j] = ft_strdup(tmp->args[i]);
			if (!res[j])
				return (ft_free(&res), NULL);
			i++;
			j++;
		}
		tmp = tmp->next;
	}
	res[j] = NULL;
	return (res);
}

char	**get_expanded_args(t_data *data, t_cmd_list *lst)
{
	t_arg_list	*head;
	char		**tmp;
	char		**ret;
	int			i;

	head = NULL;
	i = 0;
	while (lst->args[i])
	{
		tmp = expand_arg(data, lst->args[i]);
		if (!tmp)
			return (free_arg_list(&head), NULL);
		if (!add_arg_node(&head, tmp))
			return (free_arg_list(&head), ft_free(&tmp), NULL);
		ft_free(&tmp);
		i++;
	}
	ret = build_expanded_args(&head);
	free_arg_list(&head);
	return (ret);
}
