/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/12 21:32:35 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	execute_builtin_in_fork(t_command *cmd, char ***mini_env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (cmd->redir)
		{
			if (!handle_multiple_redirections(cmd->redir))
				exit(1);
		}
		exit(run_builtin(cmd->argv, mini_env, NULL));
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

static int	execute_external_command(t_command *cmd, char *command_path,
		char ***mini_env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (cmd->redir)
		{
			if (!handle_multiple_redirections(cmd->redir))
				exit(1);
		}
		execve(command_path, cmd->argv, *mini_env);
		perror("execve");
		exit(1);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

static int	execute_single_command(t_command *cmd, char ***mini_env)
{
	char	*command_path;
	int		error_code;

	if (!validate_command_args(cmd))
		return (0);
	if (is_parent_builtin(cmd->argv[0]))
		return (execute_parent_builtin(cmd, mini_env));
	if (is_builtin(cmd->argv[0]))
		return (execute_builtin_in_fork(cmd, mini_env));
	command_path = find_command_path(cmd->argv[0], *mini_env);
	error_code = check_command_path(command_path, cmd->argv[0]);
	if (error_code)
	{
		if (command_path)
			free(command_path);
		return (error_code);
	}
	error_code = check_file_permissions(command_path, cmd->argv[0]);
	if (error_code)
	{
		free(command_path);
		return (error_code);
	}
	error_code = execute_external_command(cmd, command_path, mini_env);
	return (free(command_path), error_code);
}

static int	handle_pipeline_loop(t_command *cmd_list, char ***mini_env)
{
	t_command	*current;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;

	current = cmd_list;
	prev_fd = -1;
	pid = -1;
	while (current)
	{
		if (!create_pipe_if_needed(current, pipe_fd))
			return (1);
		pid = fork();
		if (pid == 0)
			execute_child_process(current, mini_env, prev_fd, pipe_fd);
		else
			handle_parent_process(&prev_fd, pipe_fd, current);
		current = current->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (pid);
}

int	execute_pipeline(t_command *cmd_list, char ***mini_env)
{
	pid_t	last_pid;
	int		status;
	int		final_status;

	if (!cmd_list || !mini_env || !*mini_env)
		return (1);
	if (is_single_command(cmd_list))
		return (execute_single_command(cmd_list, mini_env));
	last_pid = handle_pipeline_loop(cmd_list, mini_env);
	if (last_pid == 1)
		return (1);
	final_status = 0;
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			final_status = WEXITSTATUS(status);
	}
	while (wait(NULL) > 0)
		;
	return (final_status);
}
