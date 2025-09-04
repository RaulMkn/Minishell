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

#include "../../../minishell.h"

void	reset_signal_state(void)
{
	get_signal_state()->signal_received = 0;
	get_signal_state()->in_heredoc = 0;
}

int	handle_eof_or_signal(char *line, t_shell *shell)
{
	if (!line)
	{
		if (get_signal_received() == SIGINT)
		{
			set_heredoc_state(0);
			set_signal_received(0);
			shell->last_status = 130;
			return (-1);
		}
		ft_putstr_fd("minishell: warning: document by end-of-file\n", 2);
		return (1);
	}
	return (0);
}

static void	sigint_handler_interactive(int sig)
{
	(void)sig;
	g_signal_exit_status = 130;
	set_signal_received(SIGINT);
	if (get_signal_state()->in_heredoc)
	{
		write(STDOUT_FILENO, "\n", 1);
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
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
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
