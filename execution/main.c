/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:56:11 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/20 19:11:54 by ruortiz-         ###   ########.fr       */
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

static void execute_command(t_command *cmd, char ***mini_env)
{
    pid_t pid;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return;
    if (is_builtin(cmd->argv[0]))
        run_builtin(cmd->argv, mini_env, NULL);
    else
    {
        pid = fork();
        if (pid == 0)
            execve(find_command_path(cmd->argv[0], *mini_env), cmd->argv, *mini_env);
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
                    shell->cmd_list = parse_tokens(tokens);
                    if (shell->cmd_list)
                        execute_pipeline(shell->cmd_list, &shell->envp);
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

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, envp);
    shell_loop(&shell);
    // Cleanup
    free_split(shell.envp);
    rl_clear_history();
    return (shell.last_status);
}
