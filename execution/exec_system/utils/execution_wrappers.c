/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_wrappers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 16:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	execute_parent_builtin_wrapper(t_command *cmd, char ***mini_env,
		t_shell *shell, int original_stdin)
{
	int	error_code;

	error_code = execute_parent_builtin(cmd, mini_env, shell);
	restore_stdin(original_stdin);
	return (error_code);
}

int	execute_builtin_wrapper(t_command *cmd, char ***mini_env,
		t_shell *shell, int original_stdin)
{
	int	error_code;

	error_code = execute_builtin_in_fork(cmd, mini_env, shell);
	restore_stdin(original_stdin);
	return (error_code);
}

int	execute_external_wrapper(t_command *cmd, char ***mini_env,
		t_shell *shell, int original_stdin)
{
	char	*command_path;
	int		error_code;

	command_path = find_command_path(cmd->argv[0], *mini_env);
	error_code = check_command_path(command_path, cmd->argv[0]);
	if (error_code)
	{
		restore_stdin(original_stdin);
		if (command_path)
			free(command_path);
		return (error_code);
	}
	error_code = check_file_permissions(command_path, cmd->argv[0]);
	if (error_code)
	{
		restore_stdin(original_stdin);
		free(command_path);
		return (error_code);
	}
	error_code = execute_external_command(cmd, command_path, mini_env, shell);
	restore_stdin(original_stdin);
	return (free(command_path), error_code);
}
