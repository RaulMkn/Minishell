/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:50:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:51:18 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	execute_single_child(t_cmd *cmd, char ***mini_env)
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

int	exec_single(t_cmd *cmd, char ***mini_env)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd->argv, mini_env, NULL));
	pid = fork();
	if (pid == 0)
		execute_single_child(cmd, mini_env);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
