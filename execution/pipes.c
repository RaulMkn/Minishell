/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:23:08 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/20 19:32:06 by rmakende         ###   ########.fr       */
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
		{
			if (curr->infile)
				redirect_input(curr->infile);
			else if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (curr->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (is_builtin(curr->argv[0]))
				exit(run_builtin(curr, mini_env, NULL));
			execve(find_command_path(curr->argv[0], *mini_env), curr->argv,
				*mini_env);
			perror("execve");
			exit(1);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (curr->next)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		curr = curr->next;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}
