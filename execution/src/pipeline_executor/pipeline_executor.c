/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/15 18:41:15 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	execute_builtin_in_fork(t_command *cmd, char ***mini_env,
		t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		setup_execution_signals();
		if (cmd->redir)
		{
			if (!handle_multiple_redirections(cmd->redir, shell))
				exit(1);
		}
		exit(run_builtin(cmd->argv, mini_env, NULL));
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

static int	execute_external_command(t_command *cmd, char *command_path,
		char ***mini_env, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		setup_execution_signals();
		if (cmd->redir)
		{
			if (!handle_multiple_redirections(cmd->redir, shell))
				exit(1);
		}
		execve(command_path, cmd->argv, *mini_env);
		perror("execve");
		exit(1);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	has_heredoc(t_redir *redirs)
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

int	process_all_heredocs(t_redir *redirs, t_shell *shell)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			if (handle_heredoc(current->file, shell) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

static int	execute_single_command(t_command *cmd, char ***mini_env,
		t_shell *shell)
{
	char	*command_path;
	int		error_code;
	int		original_stdin;

	original_stdin = -1;
	if (!validate_command_args(cmd))
		return (0);
	if (cmd->redir && has_heredoc(cmd->redir))
	{
		original_stdin = dup(STDIN_FILENO);
		if (process_all_heredocs(cmd->redir, shell) == -1)
		{
			if (original_stdin != -1)
			{
				dup2(original_stdin, STDIN_FILENO);
				close(original_stdin);
			}
			return (shell->last_status);
		}
	}
	if (is_parent_builtin(cmd->argv[0]))
	{
		error_code = execute_parent_builtin(cmd, mini_env, shell);
		if (original_stdin != -1)
		{
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdin);
		}
		return (error_code);
	}
	if (is_builtin(cmd->argv[0]))
	{
		error_code = execute_builtin_in_fork(cmd, mini_env, shell);
		if (original_stdin != -1)
		{
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdin);
		}
		return (error_code);
	}
	command_path = find_command_path(cmd->argv[0], *mini_env);
	error_code = check_command_path(command_path, cmd->argv[0]);
	if (error_code)
	{
		if (original_stdin != -1)
		{
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdin);
		}
		if (command_path)
			free(command_path);
		return (error_code);
	}
	error_code = check_file_permissions(command_path, cmd->argv[0]);
	if (error_code)
	{
		if (original_stdin != -1)
		{
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdin);
		}
		free(command_path);
		return (error_code);
	}
	error_code = execute_external_command(cmd, command_path, mini_env, shell);
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	return (free(command_path), error_code);
}

static int	handle_pipeline_loop(t_command *cmd_list, char ***mini_env,
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
		pid = fork();
		if (pid == 0)
		{
			p_data.prev_fd = prev_fd;
			p_data.pipe_fd = pipe_fd;
			execute_child_process(current, mini_env, &p_data, shell);
		}
		else
			handle_parent_process(&prev_fd, pipe_fd, current);
		current = current->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (pid);
}

int	execute_pipeline(t_command *cmd_list, char ***mini_env, t_shell *shell)
{
	pid_t	last_pid;
	int		status;
	int		final_status;

	if (!cmd_list || !mini_env || !*mini_env)
		return (1);
	if (is_single_command(cmd_list))
		return (execute_single_command(cmd_list, mini_env, shell));
	last_pid = handle_pipeline_loop(cmd_list, mini_env, shell);
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
