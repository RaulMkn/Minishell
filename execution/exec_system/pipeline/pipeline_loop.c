/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_loop_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 14:45:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	setup_pipe_data(t_pipe_data *p_data, int prev_fd, int *pipe_fd)
{
	p_data->prev_fd = prev_fd;
	p_data->pipe_fd = pipe_fd;
}

static pid_t	create_child_process(t_command *current, char ***mini_env,
		t_shell *shell, t_pipe_data *p_data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_child_process(current, mini_env, p_data, shell);
	return (pid);
}

static void	cleanup_pipeline(int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
}

pid_t	handle_pipeline_loop(t_command *cmd_list, char ***mini_env,
		t_shell *shell)
{
	t_command	*current;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;
	t_pipe_data	p_data;

	current = cmd_list;
	prev_fd = -1;
	pid = -1;
	while (current)
	{
		if (!create_pipe_if_needed(current, pipe_fd))
			return (1);
		setup_pipe_data(&p_data, prev_fd, pipe_fd);
		pid = create_child_process(current, mini_env, shell, &p_data);
		if (pid > 0)
			handle_parent_process(&prev_fd, pipe_fd, current);
		current = current->next;
	}
	cleanup_pipeline(prev_fd);
	return (pid);
}
