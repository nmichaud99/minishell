/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:38:14 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/23 11:38:17 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

int	word_error(int nb)
{
	if (nb == -1)
		printf("Syntax error unclosed quotes\n");
	return (0);
}

int	lex_append_char(char **txt, t_quote_type **quoting,
							char c, t_quote_type type)
{
	char			*new_txt;
	t_quote_type	*new_quoting;
	size_t			len;

	len = 0;
	if (*txt)
		len = ft_strlen(*txt);
	new_txt = malloc(len + 2);
	if (!new_txt)
		return (0);
	new_quoting = malloc(sizeof(t_quote_type) * (len + 1));
	if (!new_quoting)
		return (free(new_txt), 0);
	if (*txt)
		ft_memcpy(new_txt, *txt, len);
	new_txt[len] = c;
	new_txt[len + 1] = '\0';
	if (*quoting)
		ft_memcpy(new_quoting, *quoting, sizeof(t_quote_type) * len);
	new_quoting[len] = type;
	free(*txt);
	free(*quoting);
	*txt = new_txt;
	*quoting = new_quoting;
	return (1);
}
