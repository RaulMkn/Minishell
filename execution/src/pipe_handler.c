/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:41:31 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	setup_input_pipe(int prev_fd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
}

static void	setup_output_pipe(int *pipe_fd)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
}

void	execute_child_process(t_command *cmd, char ***mini_env, int prev_fd,
		int *pipe_fd)
{
	setup_input_pipe(prev_fd);
	if (cmd->next)
		setup_output_pipe(pipe_fd);
	if (cmd->redir)
		handle_redirections(cmd->redir);
	if (is_builtin(cmd->argv[0]))
		exit(run_builtin(cmd->argv, mini_env, NULL));
	else
	{
		execve(find_command_path(cmd->argv[0], *mini_env), cmd->argv,
			*mini_env);
		perror("execve");
		exit(1);
	}
}

void	handle_parent_process(int *prev_fd, int *pipe_fd, t_command *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}
