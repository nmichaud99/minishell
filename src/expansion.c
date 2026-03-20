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

char	*get_exit_code(t_data *data)
{
	char	*tmp_value;

	tmp_value = ft_itoa(*(data->exit_status));
	if (!tmp_value)
		return (NULL);
	return (tmp_value);
}

char	*get_variable_value(t_data *data, char *str)
{
	t_env	*tmp;
	char	*tmp_value;

	if (ft_strcmp(str, "?") == 0)
		return (get_exit_code(data));
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
		return (NULL);
	return (tmp_value);
}

int	type_of_char(char c, t_quote_type quoting, t_quote_type quote)
{
	if (c && !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
		&& quoting == quote)
		return (1);
	else if (c && ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
			|| (c >= '0' && c <= '9') || c == '_')
		&& quoting == quote)
		return (2);
	return (0);
}

char	*expand_word(t_data *data, t_word *arg, int *i, t_quote_type quote)
{
	char	*tmp;
	char	*res;
	int		count;
	int		start;

	start = *i + 1;
	count = 0;
	if (type_of_char(arg->txt[start], arg->quoting[start], quote) == 1)
		count++;
	while (type_of_char(arg->txt[start + count],
			arg->quoting[start + count], quote) == 2)
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
	*i = start + count;
	return (res);
}

static int	append_variable(char **res, char **str)
{
	char	*new_str;
	int		size;
	int		size2;

	size = ft_strlen(*res);
	size2 = ft_strlen(*str);
	new_str = malloc(size + size2 + 1);
	if (!new_str)
		return (0);
	ft_memcpy(new_str, *res, size);
	ft_memcpy(new_str + size, *str, size2);
	new_str[size + size2] = 0;
	free(*res);
	free(*str);
	*res = new_str;
	return (1);
}

static int	append_char(char **res, char c)
{
	char	*new_str;
	int		size;

	size = ft_strlen(*res);
	new_str = malloc(size + 2);
	if (!new_str)
		return (0);
	ft_memcpy(new_str, *res, size);
	new_str[size] = c;
	new_str[size + 1] = 0;
	free(*res);
	*res = new_str;
	return (1);
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
			variable = expand_word(data, arg, &i, arg->quoting[i]);
			if (!variable || !append_variable(&res, &variable))
				return (free(res), NULL);
		}
		else
		{
			if (!append_char(&res, arg->txt[i]))
				return (free(res), NULL);
			i++;
		}
	}
	return (res);
}

t_redirs	*dup_redirs(t_redirs *src)
{
	t_redirs	*head;
	t_redirs	*prev;
	t_redirs	*node;

	head = NULL;
	prev = NULL;
	while (src)
	{
		node = malloc(sizeof(t_redirs));
		if (!node)
			return (free_redirs(&head), NULL);
		node->file_name = ft_strdup(src->file_name);
		if (!node->file_name)
			return (free(node), free_redirs(&head), NULL);
		node->type = src->type;
		node->next = NULL;
		if (!head)
			head = node;
		else
			prev->next = node;
		prev = node;
		src = src->next;
	}
	return (head);
}

char	**get_expanded_args(t_data *data, t_cmd_list *lst)
{
	char	**ret;
	int		i;
	int		size;

	size = 0;
	while (lst->args[size])
		size++;
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (i < size + 1)
		ret[i++] = NULL;
	i = 0;
	while (lst->args[i])
	{
		ret[i] = expand_arg(data, lst->args[i]);
		if (!ret[i])
		{
			ft_free(&ret);
			return (NULL);
		}
		i++;
	}
	return (ret);
}

t_expanded_list	*build_expanded_list(char **expanded_args, t_cmd_list *lst)
{
	t_expanded_list	*ret;

	ret = malloc(sizeof(t_expanded_list));
	if (!ret)
	{
		ft_free(&expanded_args);
		return (NULL);
	}
	ret->args = expanded_args;
	ret->redirs = dup_redirs(lst->redirs);
	if (lst->redirs && !ret->redirs)
	{
		ft_free(&expanded_args);
		free(ret);
		return (NULL);
	}
	ret->next = NULL;
	return (ret);
}

int	expansion(t_data *data)
{
	t_cmd_list		*lst;
	char			**expanded_args;
	t_expanded_list	*expanded_list;
	t_expanded_list	*prev;

	prev = NULL;
	lst = data->cmd_list;
	while (lst)
	{
		expanded_args = get_expanded_args(data, lst);
		if (!expanded_args)
			return (free_expanded_list(&data->expanded_list), 0);
		expanded_list = build_expanded_list(expanded_args, lst);
		if (!expanded_list)
			return (free_expanded_list(&data->expanded_list), 0);
		if (!data->expanded_list)
			data->expanded_list = expanded_list;
		else
			prev->next = expanded_list;
		prev = expanded_list;
		lst = lst->next;
	}
	return (1);
}
