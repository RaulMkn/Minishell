/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 02:59:53 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

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

static t_redir	*process_heredoc_loop(t_redir *redirs, t_shell *shell,
		int original_stdin)
{
	t_redir	*current;
	t_redir	*last_heredoc;

	current = redirs;
	last_heredoc = NULL;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			last_heredoc = current;
			if (current->next && has_more_heredocs(current->next))
			{
				if (read_and_discard_heredoc(current->file, shell,
						current->no_expand) == -1)
				{
					restore_stdin(original_stdin);
					return (NULL);
				}
			}
		}
		current = current->next;
	}
	return (last_heredoc);
}

int	process_all_heredocs(t_redir *redirs, t_shell *shell)
{
	t_redir	*last_heredoc;
	int		original_stdin;

	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (-1);
	last_heredoc = process_heredoc_loop(redirs, shell, original_stdin);
	if (!last_heredoc && original_stdin != -1)
		return (-1);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	if (last_heredoc)
	{
		if (handle_heredoc(last_heredoc->file, shell,
				last_heredoc->no_expand) == -1)
			return (-1);
	}
	return (0);
}

static int	execute_single_command(t_command *cmd, char ***mini_env,
		t_shell *shell)
{
	int		error_code;
	int		original_stdin;

	original_stdin = -1;
	error_code = setup_heredocs(cmd, shell, &original_stdin);
	if (error_code == -1)
		return (130);
	if (error_code)
		return (error_code);
	if (!validate_command_args(cmd))
		return (0);
	if (is_parent_builtin(cmd->argv[0]))
		return (execute_parent_builtin_wrapper(cmd, mini_env, shell,
				original_stdin));
	if (is_builtin(cmd->argv[0]))
		return (execute_builtin_wrapper(cmd, mini_env, shell, original_stdin));
	return (execute_external_wrapper(cmd, mini_env, shell, original_stdin));
}

int	execute_pipeline(t_command *cmd_list, char ***mini_env, t_shell *shell)
{
	pid_t		last_pid;
	int			final_status;
	void		(*old_sigint)(int);
	int			original_stdin;

	if (!cmd_list || !mini_env || !*mini_env)
		return (1);
	if (is_single_command(cmd_list))
		return (execute_single_command(cmd_list, mini_env, shell));
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (1);
	old_sigint = signal(SIGINT, SIG_IGN);
	last_pid = handle_pipeline_loop(cmd_list, mini_env, shell);
	if (last_pid == 1)
	{
		signal(SIGINT, old_sigint);
		restore_stdin(original_stdin);
		return (1);
	}
	final_status = wait_for_pipeline_completion(last_pid);
	signal(SIGINT, old_sigint);
	restore_stdin(original_stdin);
	return (final_status);
}
