/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/14 17:11:45 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_error_and_exit(char *cmd_name, char *msg, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(msg, 2);
	exit(exit_code);
}

static void	validate_command_path(char *command_path, char *cmd_name)
{
	struct stat	path_stat;

	if (!command_path)
		print_error_and_exit(cmd_name, ": command not found\n", 127);
	if (access(command_path, F_OK) != 0)
	{
		free(command_path);
		print_error_and_exit(cmd_name, ": No such file or directory\n", 127);
	}
	if (stat(command_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		free(command_path);
		print_error_and_exit(cmd_name, ": Is a directory\n", 126);
	}
	if (access(command_path, X_OK) != 0)
	{
		free(command_path);
		print_error_and_exit(cmd_name, ": Permission denied\n", 126);
	}
}

static void	validate_and_execute_external(t_command *cmd, char ***mini_env)
{
	char	*command_path;

	command_path = find_command_path(cmd->argv[0], *mini_env);
	validate_command_path(command_path, cmd->argv[0]);
	execve(command_path, cmd->argv, *mini_env);
	perror("execve");
	free(command_path);
	exit(1);
}

void	execute_child_process(t_command *cmd, char ***mini_env, int prev_fd,
		int *pipe_fd, t_shell *shell)
{
	setup_execution_signals();
	setup_input_pipe(prev_fd);
	if (cmd->next)
		setup_output_pipe(pipe_fd);
	if (cmd->redir)
	{
		if (!handle_multiple_redirections(cmd->redir, shell))
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
		validate_and_execute_external(cmd, mini_env);
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
