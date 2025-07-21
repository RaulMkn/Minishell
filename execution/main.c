/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:56:11 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:27:07 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_shell(t_shell *shell, char **envp)
{
	shell->cmd_list = NULL;
	shell->envp = clone_env(envp);
	shell->last_status = 0;
	shell->lexer_state.quote_state = QUOTE_NONE;
	shell->lexer_state.error = ERROR_NONE;
	shell->lexer_state.error_msg = NULL;
}

static void	handle_redirections(t_redir *redirs)
{
	t_redir	*current;
	int		fd;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_OUT)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd != -1)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
		else if (current->type == REDIR_IN)
		{
			fd = open(current->file, O_RDONLY);
			if (fd != -1)
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		else if (current->type == REDIR_APPEND)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd != -1)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
		current = current->next;
	}
}

static void	execute_pipeline(t_command *cmd_list, char ***mini_env)
{
	t_command	*current;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;

	current = cmd_list;
	prev_fd = -1;
	while (current)
	{
		if (current->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return ;
		}
		pid = fork();
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (current->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}
			if (current->redir)
				handle_redirections(current->redir);
			if (is_builtin(current->argv[0]))
			{
				exit(run_builtin(current->argv, mini_env, NULL));
			}
			else
			{
				execve(find_command_path(current->argv[0], *mini_env),
					current->argv, *mini_env);
				perror("execve");
				exit(1);
			}
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (current->next)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
		}
		current = current->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	while (wait(NULL) > 0)
		;
}

static void	shell_loop(t_shell *shell)
{
	char		*line;
	t_token		*tokens;
	t_command	*new_cmds;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			tokens = tokenize_input(line, shell);
			if (shell->lexer_state.error == ERROR_NONE && tokens)
			{
				if (is_valid_operator_sequence(tokens))
				{
					new_cmds = parse_tokens(tokens);
					if (new_cmds)
					{
						if (shell->cmd_list)
							clear_command(shell->cmd_list);
						shell->cmd_list = new_cmds;
						execute_pipeline(shell->cmd_list, &shell->envp);
						shell->last_status = 0;
					}
					else
						ft_putendl_fd("Error en el parseo de comandos", 2);
				}
				else
					ft_putendl_fd("Error de sintaxis", 2);
				clear_tokens(&tokens);
			}
			else if (shell->lexer_state.error_msg)
				ft_putendl_fd(shell->lexer_state.error_msg, 2);
		}
		free(line);
	}
}

static void	cleanup_shell(t_shell *shell)
{
	t_command	*current;
	t_command	*next;

	if (shell->envp)
		free_split(shell->envp);
	current = shell->cmd_list;
	while (current)
	{
		next = current->next;
		clear_command(current);
		current = next;
	}
	if (shell->lexer_state.error_msg)
		free(shell->lexer_state.error_msg);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	shell_loop(&shell);
	cleanup_shell(&shell);
	rl_clear_history();
	return (shell.last_status);
}
