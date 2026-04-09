/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjerrige <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:09:07 by fjerrige          #+#    #+#             */
/*   Updated: 2026/04/09 16:09:08 by fjerrige         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_signals_exec(void)
{
	signal(SIGINT, sigint_handler_exec);
	signal(SIGQUIT, sigquit_handler);
	signal(SIGPIPE, SIG_DFL);
}

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	g_signalstatus = SIGINT;
	close(STDIN_FILENO);
}
