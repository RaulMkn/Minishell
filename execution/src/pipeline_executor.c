/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:41:01 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_pipe_if_needed(t_command *current, int *pipe_fd)
{
	if (current->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

static void	wait_for_children(void)
{
	while (wait(NULL) > 0)
		;
}

void	execute_pipeline(t_command *cmd_list, char ***mini_env)
{
	t_command	*current;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;

	current = cmd_list;
	prev_fd = -1;
	while (current)
	{
		if (!create_pipe_if_needed(current, pipe_fd))
			return ;
		pid = fork();
		if (pid == 0)
			execute_child_process(current, mini_env, prev_fd, pipe_fd);
		else
			handle_parent_process(&prev_fd, pipe_fd, current);
		current = current->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_for_children();
}
