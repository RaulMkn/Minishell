/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/29 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	wait_for_pipeline_completion(pid_t last_pid)
{
	int		status;
	int		final_status;

	final_status = 0;
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			final_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			final_status = 128 + WTERMSIG(status);
	}
	while (wait(NULL) > 0)
		;
	return (final_status);
}

void	restore_stdin(int original_stdin)
{
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
}

void	cleanup_pipeline(int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
}
