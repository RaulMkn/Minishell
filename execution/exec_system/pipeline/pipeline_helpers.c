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

void	restore_stdin(int original_stdin)
{
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
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
