/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:56:11 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/20 21:12:21 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void init_shell(t_shell *shell, char **envp)
{
    shell->cmd_list = NULL;
    shell->envp = clone_env(envp);
    shell->last_status = 0;
    shell->lexer_state.quote_state = QUOTE_NONE;
    shell->lexer_state.error = ERROR_NONE;
    shell->lexer_state.error_msg = NULL;
}

static void handle_redirections(t_redir *redirs)
{
    t_redir *current;

    current = redirs;
    while (current)
    {
        if (current->type == REDIR_OUT)
        {
            int fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd != -1)
            {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
        }
        else if (current->type == REDIR_IN)
        {
            int fd = open(current->file, O_RDONLY);
            if (fd != -1)
            {
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
        }
        else if (current->type == REDIR_APPEND)
        {
            int fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd != -1)
            {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
        }
        current = current->next;
    }
}

static void execute_command(t_command *cmd, char ***mini_env)
{
    pid_t pid;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

    if (is_builtin(cmd->argv[0]))
    {
        // Para builtins, guardamos los fd originales
        int stdout_backup = dup(STDOUT_FILENO);
        int stdin_backup = dup(STDIN_FILENO);
        
        if (cmd->redir)
            handle_redirections(cmd->redir);
            
        run_builtin(cmd->argv, mini_env, NULL);
        
        // Restauramos los fd originales
        dup2(stdout_backup, STDOUT_FILENO);
        dup2(stdin_backup, STDIN_FILENO);
        close(stdout_backup);
        close(stdin_backup);
    }
    else
    {
        pid = fork();
        if (pid == 0)
        {
            if (cmd->redir)
                handle_redirections(cmd->redir);
            execve(find_command_path(cmd->argv[0], *mini_env), cmd->argv, *mini_env);
            exit(1);  // En caso de error en execve
        }
        else
            wait(NULL);
    }
}

static void execute_pipeline(t_command *cmd_list, char ***mini_env)
{
    t_command *current;

    current = cmd_list;
    while (current)
    {
        execute_command(current, mini_env);
        current = current->next;
    }
}

static void shell_loop(t_shell *shell)
{
    char *line;
    t_token *tokens;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            break;
        if (*line)
        {
            add_history(line);
            tokens = tokenize_input(line, shell);
            if (shell->lexer_state.error == ERROR_NONE && tokens)
            {
                if (is_valid_operator_sequence(tokens))
                {
                    t_command *new_cmds = parse_tokens(tokens);
                    if (new_cmds)
                    {
                        // Limpiar la lista anterior si existe
                        if (shell->cmd_list)
                            clear_command(shell->cmd_list);
                        shell->cmd_list = new_cmds;
                        execute_pipeline(shell->cmd_list, &shell->envp);
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

static void cleanup_shell(t_shell *shell)
{
    t_command *current;
    t_command *next;

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

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, envp);
    shell_loop(&shell);
    cleanup_shell(&shell);
    rl_clear_history();
    return (shell.last_status);
}
