/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/28 21:25:28 by rmakende         ###   ########.fr       */
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
	char	*command_path;
		struct stat path_stat;

	setup_input_pipe(prev_fd);
	if (cmd->next)
		setup_output_pipe(pipe_fd);
	if (cmd->redir)
	{
		if (!handle_redirections(cmd->redir))
			exit(1);
	}
	if (!cmd->argv || !cmd->argv[0] || !cmd->argv[0][0])
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		exit(127);
	}
	if (is_builtin(cmd->argv[0]))
		exit(run_builtin(cmd->argv, mini_env, NULL));
	else
	{
		command_path = find_command_path(cmd->argv[0], *mini_env);
		if (!command_path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		if (access(command_path, F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(command_path);
			exit(127);
		}
		if (stat(command_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			free(command_path);
			exit(126);
		}
		if (access(command_path, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			free(command_path);
			exit(126);
		}
		execve(command_path, cmd->argv, *mini_env);
		perror("execve");
		free(command_path);
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
