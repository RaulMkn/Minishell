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

int	get_heredoc_state(void)
{
	return (get_signal_state()->in_heredoc);
}

void	reset_signal_state(void)
{
	get_signal_state()->signal_received = 0;
	get_signal_state()->in_heredoc = 0;
}

static void	sigint_handler_interactive(int sig)
{
	(void)sig;
	set_signal_received(SIGINT);
	if (get_heredoc_state())
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
	}
	else
	{
		write(STDOUT_FILENO, "\n", 1);
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
