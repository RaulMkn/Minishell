/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_processes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:30:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 18:41:33 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	handle_process_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int	execute_builtin_in_fork(t_command *cmd, char ***mini_env, t_shell *shell)
{
	pid_t		pid;
	int			status;
	void		(*old_sigint)(int);

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
	old_sigint = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_sigint);
	return (handle_process_status(status));
}

int	execute_external_command(t_command *cmd, char *command_path,
		char ***mini_env, t_shell *shell)
{
	pid_t		pid;
	int			status;
	void		(*old_sigint)(int);

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
	old_sigint = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_sigint);
	return (handle_process_status(status));
}
