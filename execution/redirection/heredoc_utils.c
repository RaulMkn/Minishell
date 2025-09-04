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

static char	*read_heredoc_with_signals(void)
{
	fd_set		readfds;
	struct timeval	timeout;
	char		*line;
	int		ready;

	write(STDOUT_FILENO, "> ", 2);
	while (1)
	{
		if (get_signal_received() == SIGINT)
			return (NULL);
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000;
		ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
		if (ready > 0 && FD_ISSET(STDIN_FILENO, &readfds))
		{
			line = get_next_line(STDIN_FILENO);
			return (line);
		}
		if (ready == -1 && errno != EINTR)
			return (NULL);
	}
}

char	*read_heredoc_line(void)
{
	if (isatty(STDIN_FILENO))
		return (read_heredoc_with_signals());
	else
		return (get_next_line(STDIN_FILENO));
}

int	process_heredoc_input_line(t_heredoc_ctx *ctx)
{
	char	*expanded_line;

	if (ft_strlen(ctx->line) > 0 && ctx->line[ft_strlen(ctx->line) - 1] == '\n')
		ctx->line[ft_strlen(ctx->line) - 1] = '\0';
	if (ft_strcmp(ctx->line, ctx->delimiter) == 0)
	{
		free(ctx->line);
		set_heredoc_state(0);
		return (0);
	}
	if (ctx->no_expand)
		expanded_line = ft_strdup(ctx->line);
	else
		expanded_line = expand_variables(ctx->line, ctx->shell->envp,
				ctx->shell->last_status, 0);
	if (!expanded_line)
		expanded_line = ft_strdup("");
	write(ctx->fd, expanded_line, ft_strlen(expanded_line));
	write(ctx->fd, "\n", 1);
	free(expanded_line);
	free(ctx->line);
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

int	process_single_line(int fd, char *delimiter, t_shell *shell,
		int no_expand)
{
	char			*line;
	int				eof_status;
	t_heredoc_ctx	ctx;

	if (check_signal_interrupt(NULL, shell) == -1)
		return (-1);
	line = read_heredoc_line();
	if (check_signal_interrupt(line, shell) == -1)
		return (-1);
	eof_status = handle_eof_or_signal(line, shell);
	if (eof_status == -1)
		return (-1);
	if (eof_status == 1)
		return (0);
	ctx.fd = fd;
	ctx.line = line;
	ctx.delimiter = delimiter;
	ctx.shell = shell;
	ctx.no_expand = no_expand;
	return (process_heredoc_input_line(&ctx));
}
