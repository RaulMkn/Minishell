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

#include "../../../minishell.h"

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

static int	process_command_heredocs(t_command *current, t_shell *shell)
{
	if (current->redir && has_heredoc(current->redir))
	{
		if (process_all_heredocs(current->redir, shell) == -1)
			return (-1);
	}
	return (0);
}

static pid_t	process_single_command(t_command *current, char ***mini_env,
		t_shell *shell, int *prev_fd)
{
	int			pipe_fd[2];
	pid_t		pid;
	t_pipe_data	p_data;

	if (!create_pipe_if_needed(current, pipe_fd))
		return (-1);
	setup_pipe_data(&p_data, *prev_fd, pipe_fd);
	pid = create_child_process(current, mini_env, shell, &p_data);
	if (pid > 0)
		handle_parent_process(prev_fd, pipe_fd, current);
	return (pid);
}

pid_t	handle_pipeline_loop(t_command *cmd_list, char ***mini_env,
		t_shell *shell)
{
	t_command	*current;
	int			prev_fd;
	pid_t		pid;

	current = cmd_list;
	prev_fd = -1;
	pid = -1;
	while (current)
	{
		if (process_command_heredocs(current, shell) == -1)
			return (1);
		pid = process_single_command(current, mini_env, shell, &prev_fd);
		if (pid == -1)
			return (1);
		current = current->next;
	}
	cleanup_pipeline(prev_fd);
	return (pid);
}
