/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:50:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:52:31 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	setup_child_input(t_cmd *curr, int in_fd)
{
	if (curr->infile)
		redirect_input(curr->infile);
	else if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
}

void	setup_child_output(t_cmd *curr, int *pipe_fd)
{
	if (curr->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	execute_child_command(t_cmd *curr, char ***mini_env)
{
	if (is_builtin(curr->argv[0]))
		exit(run_builtin(curr->argv, mini_env, NULL));
	execve(find_command_path(curr->argv[0], *mini_env), curr->argv, *mini_env);
	perror("execve");
	exit(1);
}

void	handle_child_process(t_cmd *curr, char ***mini_env, int in_fd,
		int *pipe_fd)
{
	setup_child_input(curr, in_fd);
	setup_child_output(curr, pipe_fd);
	execute_child_command(curr, mini_env);
}
