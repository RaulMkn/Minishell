/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/29 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*read_heredoc_line(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	else
		return (get_next_line(STDIN_FILENO));
}

int	process_heredoc_input_line(int fd, char *line, char *delimiter, 
		t_shell *shell)
{
	char	*expanded_line;

	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		set_heredoc_state(0);
		return (0);
	}
	expanded_line = expand_variables(line, shell->envp, shell->last_status, 0);
	if (!expanded_line)
		expanded_line = ft_strdup("");
	write(fd, expanded_line, ft_strlen(expanded_line));
	write(fd, "\n", 1);
	free(expanded_line);
	free(line);
	return (1);
}

int	check_signal_interrupt(char *line, t_shell *shell)
{
	if (get_signal_received() == SIGINT)
	{
		if (line)
			free(line);
		set_heredoc_state(0);
		set_signal_received(0);
		shell->last_status = 130;
		return (-1);
	}
	return (0);
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
