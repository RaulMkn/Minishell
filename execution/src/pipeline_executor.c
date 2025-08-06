/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/06 19:17:31 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_pipe_if_needed(t_command *current, int *pipe_fd)
{
	if (current->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

static int	is_single_command(t_command *cmd_list)
{
	return (cmd_list && !cmd_list->next);
}

static int	execute_parent_builtin(t_command *cmd, char ***mini_env)
{
	int	original_stdin;
	int	original_stdout;
	int	result;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (cmd->redir)
	{
		if (!handle_multiple_redirections(cmd->redir))
		{
			dup2(original_stdin, STDIN_FILENO);
			dup2(original_stdout, STDOUT_FILENO);
			close(original_stdin);
			close(original_stdout);
			return (1);
		}
	}
	result = run_builtin(cmd->argv, mini_env, NULL);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	return (result);
}

static void	wait_for_children(void)
{
	while (wait(NULL) > 0)
		;
}
static int	execute_single_command(t_command *cmd, char ***mini_env)
{
    char		*command_path;
    pid_t		pid;
    int			status;
    //struct stat	path_stat;

    // Verificar que el comando no sea NULL o vacío
    if (!cmd || !cmd->argv || !cmd->argv[0] || !cmd->argv[0][0])
    {
        ft_putstr_fd("minishell: : command not found\n", 2);
        return (127);
    }
    if (is_parent_builtin(cmd->argv[0]))
        return (execute_parent_builtin(cmd, mini_env));
    if (is_builtin(cmd->argv[0]))
    {
        // Builtins que pueden ejecutarse en procesos hijos
        pid = fork();
        if (pid == 0)
        {
            if (cmd->redir)
            {
                if (!handle_multiple_redirections(cmd->redir))
                    exit(1);
            }
            // 🔧 SOLO ejecutar builtin, NO execve
            exit(run_builtin(cmd->argv, mini_env, NULL));
        }
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
    
    // 🔧 COMANDO EXTERNO - Aquí inicializar command_path
    command_path = find_command_path(cmd->argv[0], *mini_env);
    if (!command_path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        return (127);
    }
    // ...existing code...
    pid = fork();
    if (pid == 0)
    {
        if (cmd->redir)
        {
            if (!handle_multiple_redirections(cmd->redir))
                exit(1);
        }
        execve(command_path, cmd->argv, *mini_env);
        perror("execve");
        exit(1);
    }
    free(command_path);
    waitpid(pid, &status, 0);
    return (WEXITSTATUS(status));
}

int	execute_pipeline(t_command *cmd_list, char ***mini_env)
{
	t_command	*current;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;
	int			status;

	// Verificaciones de seguridad
	if (!cmd_list || !mini_env || !*mini_env)
		return (1);
	// Caso especial: comando único
	if (is_single_command(cmd_list))
	{
		return (execute_single_command(cmd_list, mini_env));
	}
	// Pipeline con múltiples comandos
	current = cmd_list;
	prev_fd = -1;
	pid = -1; // Para guardar el PID del último comando
	while (current)
	{
		if (!create_pipe_if_needed(current, pipe_fd))
			return (1);
		pid = fork();
		if (pid == 0)
			execute_child_process(current, mini_env, prev_fd, pipe_fd);
		else
			handle_parent_process(&prev_fd, pipe_fd, current);
		current = current->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	// Esperar al último proceso del pipeline
	if (pid > 0)
		waitpid(pid, &status, 0);
	wait_for_children();
	return (WEXITSTATUS(status));
}
