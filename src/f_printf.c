/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_printf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 09:13:26 by nmichaud          #+#    #+#             */
/*   Updated: 2026/03/05 09:13:41 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void    f_printf(char *s1, char *s2)
{
    int size1;
    int size2;

    write(2, "minishell: ", 11);
    size1 = ft_strlen(s1);
    size2 = ft_strlen(s2);
    write(2, s1, size1);
    write(2, ": ", 2);
    write(2, s2, size2);
}
