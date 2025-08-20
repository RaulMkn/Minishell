/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:51:51 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 15:18:35 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	create_pipe_if_needed(t_command *current, int *pipe_fd)
{
	if (current->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

int	is_single_command(t_command *cmd_list)
{
	return (cmd_list && !cmd_list->next);
}

int	execute_parent_builtin(t_command *cmd, char ***mini_env, t_shell *shell)
{
	int	original_stdin;
	int	original_stdout;
	int	result;

	if (!cmd->redir)
		return (run_builtin(cmd->argv, mini_env, NULL));
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdin == -1 || original_stdout == -1)
	{
		if (original_stdin != -1)
			close(original_stdin);
		if (original_stdout != -1)
			close(original_stdout);
		return (1);
	}
	if (!handle_multiple_redirections(cmd->redir, shell))
	{
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		return (1);
	}
	result = run_builtin(cmd->argv, mini_env, NULL);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	return (result);
}
