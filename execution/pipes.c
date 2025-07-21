/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:23:08 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:51:18 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_dispatch(t_cmd *cmd, char ***mini_env)
{
	if (!cmd)
		return (1);
	if (cmd->next == NULL)
		return (exec_single(cmd, mini_env));
	else
		return (exec_pipeline(cmd, mini_env));
}

int	exec_single(t_cmd *cmd, char ***mini_env)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd, mini_env, NULL));
	pid = fork();
	if (pid == 0)
	{
		if (cmd->infile)
			redirect_input(cmd->infile);
		if (cmd->outfile)
			redirect_output(cmd->outfile, cmd->append);
		execve(find_command_path(cmd->argv[0], *mini_env), cmd->argv,
			*mini_env);
		perror("execve");
		exit(1);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

static void	setup_child_input(t_cmd *curr, int in_fd)
{
	if (curr->infile)
		redirect_input(curr->infile);
	else if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
}

static void	setup_child_output(t_cmd *curr, int *pipe_fd)
{
	if (curr->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

static void	execute_child_command(t_cmd *curr, char ***mini_env)
{
	if (is_builtin(curr->argv[0]))
		exit(run_builtin(curr, mini_env, NULL));
	execve(find_command_path(curr->argv[0], *mini_env), curr->argv,
		*mini_env);
	perror("execve");
	exit(1);
}

static void	handle_child_process(t_cmd *curr, char ***mini_env, int in_fd, int *pipe_fd)
{
	setup_child_input(curr, in_fd);
	setup_child_output(curr, pipe_fd);
	execute_child_command(curr, mini_env);
}

static int	handle_parent_process(int in_fd, int *pipe_fd, t_cmd *curr)
{
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (curr->next)
	{
		close(pipe_fd[1]);
		return (pipe_fd[0]);
	}
	return (STDIN_FILENO);
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
		in_fd = handle_parent_process(in_fd, pipe_fd, curr);
		curr = curr->next;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}
