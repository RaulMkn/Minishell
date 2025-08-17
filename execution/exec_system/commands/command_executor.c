/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:50:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:51:18 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	wait_for_all_children(void)
{
	while (wait(NULL) > 0)
		;
}

int	exec_pipeline(t_cmd *cmd, char ***mini_env)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	t_cmd	*curr;

	in_fd = STDIN_FILENO;
	curr = cmd;
	while (curr)
	{
		if (curr->next && pipe(pipe_fd) < 0)
			perror_exit("pipe");
		pid = fork();
		if (pid == 0)
			handle_child_process(curr, mini_env, in_fd, pipe_fd);
		in_fd = handle_pipe_parent_process(in_fd, pipe_fd, curr);
		curr = curr->next;
	}
	wait_for_all_children();
	return (0);
}
