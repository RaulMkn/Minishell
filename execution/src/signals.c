/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/08/14 17:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;
volatile sig_atomic_t	g_in_heredoc = 0;

static void	sigint_handler_interactive(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	if (!g_in_heredoc)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;

	signal(SIGQUIT, SIG_IGN);
	sa_int.sa_handler = sigint_handler_interactive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
}

void	setup_execution_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

