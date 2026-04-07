/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:40:56 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/27 12:40:57 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_message(char *s)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("'\n", 2);
}

int	handle_double_redirs(t_token *tmp)
{
	if (tmp->next->type == IN_DIR)
		return (print_message("<"), 0);
	else if (tmp->next->type == OUT_DIR)
		return (print_message(">"), 0);
	else if (tmp->next->type == HEREDOC)
		return (print_message("<<"), 0);
	else if (tmp->next->type == APPEND)
		return (print_message(">>"), 0);
	return (1);
}

int	syntax_check(t_data *data)
{
	t_token	*tmp;

	tmp = data->tokens;
	if (!tmp)
		return (1);
	if (tmp->type == PIPE)
		return (print_message("|"), 0);
	while (tmp)
	{
		if (tmp->type == PIPE && tmp->next && tmp->next->type == PIPE)
			return (print_message("|"), 0);
		else if (tmp->type == PIPE && !tmp->next)
			return (print_message("newline"), 0);
		else if (is_redir(tmp->type) && tmp->next && is_redir(tmp->next->type))
		{
			if (!handle_double_redirs(tmp))
				return (0);
		}
		else if (is_redir(tmp->type) && tmp->next && tmp->next->type == PIPE)
			return (print_message("|"), 0);
		else if (is_redir(tmp->type) && !tmp->next)
			return (print_message("newline"), 0);
		tmp = tmp->next;
	}
	return (1);
}
