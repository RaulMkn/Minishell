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

static int	has_more_heredocs(t_redir *redirs)
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

static int	read_and_discard_heredoc(char *delimiter, t_shell *shell)
{
	char	*line;

	(void)shell;
	set_heredoc_state(1);
	while (1)
	{
		line = readline("> ");
		if (get_signal_received() == SIGINT)
			return (set_heredoc_state(0), free(line), -1);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: document by end-of-file\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
	set_heredoc_state(0);
	return (0);
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
	t_redir	*last_heredoc;
	int		original_stdin;

	current = redirs;
	last_heredoc = NULL;
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (-1);
	while (current)
	{
		if (current->type == HEREDOC)
		{
			last_heredoc = current;
			if (current->next && has_more_heredocs(current->next))
			{
				if (read_and_discard_heredoc(current->file, shell) == -1)
				{
					close(original_stdin);
					return (-1);
				}
			}
		}
		current = current->next;
	}
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	if (last_heredoc)
	{
		if (handle_heredoc(last_heredoc->file, shell) == -1)
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

static int	wait_for_pipeline_completion(pid_t last_pid)
{
	int		status;
	int		final_status;

	final_status = 0;
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			final_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			final_status = 128 + WTERMSIG(status);
	}
	while (wait(NULL) > 0)
		;
	return (final_status);
}

int	execute_pipeline(t_command *cmd_list, char ***mini_env, t_shell *shell)
{
	pid_t		last_pid;
	int			final_status;
	void		(*old_sigint)(int);

	if (!cmd_list || !mini_env || !*mini_env)
		return (1);
	if (is_single_command(cmd_list))
		return (execute_single_command(cmd_list, mini_env, shell));
	old_sigint = signal(SIGINT, SIG_IGN);
	last_pid = handle_pipeline_loop(cmd_list, mini_env, shell);
	if (last_pid == 1)
	{
		signal(SIGINT, old_sigint);
		return (1);
	}
	final_status = wait_for_pipeline_completion(last_pid);
	signal(SIGINT, old_sigint);
	return (final_status);
}
