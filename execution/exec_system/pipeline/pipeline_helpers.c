/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:30:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 14:30:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	has_more_heredocs(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == HEREDOC)
			return (1);
		current = current->next;
	}
	return (0);
}

static char	*read_line_from_input(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	else
		return (get_next_line(STDIN_FILENO));
}

static int	process_heredoc_line(char *line, char *delimiter)
{
	if (get_signal_received() == SIGINT)
		return (free(line), -1);
	if (!line)
	{
		ft_putstr_fd("minishell: warning: document by end-of-file\n", 2);
		return (1);
	}
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	free(line);
	return (0);
}

int	read_and_discard_heredoc(char *delimiter, t_shell *shell)
{
	char	*line;
	int		status;

	(void)shell;
	set_heredoc_state(1);
	while (1)
	{
		line = read_line_from_input();
		status = process_heredoc_line(line, delimiter);
		if (status == -1)
			return (set_heredoc_state(0), -1);
		if (status == 1)
			break ;
	}
	set_heredoc_state(0);
	return (0);
}

int	setup_heredocs(t_command *cmd, t_shell *shell, int *original_stdin)
{
	if (cmd->redir && has_heredoc(cmd->redir))
	{
		*original_stdin = dup(STDIN_FILENO);
		if (*original_stdin == -1)
		{
			perror("dup");
			return (1);
		}
		if (process_all_heredocs(cmd->redir, shell) == -1)
		{
			restore_stdin(*original_stdin);
			return (shell->last_status);
		}
	}
	return (0);
}
