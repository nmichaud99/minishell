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

char	*get_variable_value(t_data *data, char *str)
{
	t_env	*tmp;
	char	*tmp_value;

	if (ft_strcmp(str, "?") == 0)
	{
		tmp_value = ft_itoa(*(data->exit_status));
		if (!tmp_value)
			exit_free(data, EXIT_FAILURE);
		return (tmp_value);
	}
	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, str) == 0)
		{
			tmp_value = ft_strdup(tmp->value);
			if (!tmp_value)
				return (NULL);
			return (tmp_value);
		}
		tmp = tmp->next;
	}
	tmp_value = ft_strdup("");
	if (!tmp_value)
		exit_free(data, EXIT_FAILURE);
	return (tmp_value);
}

char	*expand(t_data *data, t_word *arg, int *i, t_quote_type quote)
{
	char	*tmp;
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	count = 0;
	if (arg->txt[start] &&
           !((arg->txt[start] >= 'A' && arg->txt[start] <= 'Z') ||
            (arg->txt[start] >= 'a' && arg->txt[start] <= 'z') ||
            arg->txt[start] == '_') && arg->quoting[start] == quote)
		count++;
	else
	{
		while (arg->txt[start + count] &&
			   ((arg->txt[start + count] >= 'A' && arg->txt[start + count] <= 'Z') ||
				(arg->txt[start + count] >= 'a' && arg->txt[start + count] <= 'z') ||
				(arg->txt[start + count] >= '0' && arg->txt[start + count] <= '9') ||
				arg->txt[start + count] == '_') && arg->quoting[start + count] == quote)
			count++;
	}
	tmp = malloc(count + 1);
	if (!tmp)
		exit_free(data, EXIT_FAILURE);
	ft_memcpy(tmp, &arg->txt[start], count);
	tmp[count] = 0;
	res = get_variable_value(data, tmp);
	free(tmp);
	if (!res)
		exit_free(data, EXIT_FAILURE);
	*i = start + count;
	return (res);
}

static void	append_variable(t_data *data, char **res, char **str)
{
	char	*new_str;
	int		size;
	int		size2;

	size = ft_strlen(*res);
	size2 = ft_strlen(*str);
	new_str = malloc(size + size2 + 1);
	if (!new_str)
		exit_free(data, EXIT_FAILURE);
	ft_memcpy(new_str, *res, size);
	ft_memcpy(new_str + size, *str, size2);
	new_str[size + size2] = 0;
	free(*res);
	free(*str);
	*res = new_str;
}

static void	append_char(t_data *data, char **res, char c)
{
	char	*new_str;
	int		size;

	size = ft_strlen(*res);
	new_str = malloc(size + 2);
	if (!new_str)
		exit_free(data, EXIT_FAILURE);
	ft_memcpy(new_str, *res, size);
	new_str[size] = c;
	new_str[size + 1] = 0;
	free(*res);
	*res = new_str;
}

char	*expand_arg(t_data *data, t_word *arg)
{
	char			*res;
	char			*variable;
	int				i;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (arg->txt[i])
	{
		if (arg->txt[i] == '$' && arg->quoting[i] != SINGLE)
		{
			variable = expand(data, arg, &i, arg->quoting[i]);
			if (!variable)
			{
				free(res);
				return (NULL);
			}
			append_variable(data, &res, &variable);
		}
		else
		{
			append_char(data, &res, arg->txt[i]);
			i++;
		}
	}
	return (res);
}

t_redirs	*dup_redirs(t_redirs *src)
{
	t_redirs *head = NULL;
	t_redirs *prev = NULL;

	while (src)
	{
		t_redirs *node = malloc(sizeof(t_redirs));
		if (!node)
			return NULL;
		node->file_name = ft_strdup(src->file_name);
		node->type = src->type;
		node->next = NULL;
		if (!head)
			head = node;
		else
			prev->next = node;
		prev = node;
		src = src->next;
	}
	return head;
}

void	expansion(t_data *data)
{
	t_cmd_list		*lst;
	t_word			**args;
	char			**expanded_args;
	t_expanded_list	*expanded;
	t_expanded_list	*prev;
	int				i;

	prev = NULL;
	lst = data->cmd_list;
	while (lst)
	{
		args = lst->args;
		i = 0;
		while (args[i])
			i++;
		expanded_args = malloc(sizeof(char *) * (i + 1));
		if (!expanded_args)
			exit_free(data, EXIT_FAILURE);
		args = lst->args;
		i = 0;
		while (args[i])
		{
			expanded_args[i] = expand_arg(data, args[i]);
			if (!expanded_args[i])
			{
				ft_free(&expanded_args);
				exit_free(data, EXIT_FAILURE);
			}
			i++;
		}
		expanded_args[i] = NULL;
		expanded = malloc(sizeof(t_expanded_list));
		if (!expanded)
		{
			ft_free(&expanded_args);
			exit_free(data, EXIT_FAILURE);
		}
		expanded->args = expanded_args;
		expanded->redirs = dup_redirs(lst->redirs);
		if (lst->redirs && !expanded->redirs)
			exit_free(data, EXIT_FAILURE);
		expanded->next = NULL;
		if (!data->expanded_list)
			data->expanded_list = expanded;
		else
			prev->next = expanded;
		prev = expanded;
		lst = lst->next;
	}
}
