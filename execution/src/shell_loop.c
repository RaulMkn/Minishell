/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/24 23:57:13 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_valid_tokens(t_shell *shell, t_token *tokens)
{
	t_command	*new_cmds;

	if (is_valid_operator_sequence(tokens))
	{
		new_cmds = parse_tokens(tokens);
		if (new_cmds)
		{
			if (shell->cmd_list)
				clear_command(shell->cmd_list);
			shell->cmd_list = new_cmds;
			shell->last_status = execute_pipeline(shell->cmd_list,
					&shell->envp);
		}
		else
			ft_putendl_fd("Error en el parseo de comandos", 2);
	}
	else
		ft_putendl_fd("Error de sintaxis", 2);
}

static void	process_input_line(t_shell *shell, char *line)
{
	t_token	*tokens;

	add_history(line);
	tokens = tokenize_input(line, shell);
	if (shell->lexer_state.error == ERROR_NONE && tokens)
	{
		process_valid_tokens(shell, tokens);
		clear_tokens(&tokens);
	}
	else if (shell->lexer_state.error_msg)
		ft_putendl_fd(shell->lexer_state.error_msg, 2);
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			process_input_line(shell, line);
		free(line);
	}
}
