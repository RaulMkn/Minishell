/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:41:02 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	shell->cmd_list = NULL;
	shell->envp = clone_env(envp);
	shell->last_status = 0;
	shell->lexer_state.quote_state = QUOTE_NONE;
	shell->lexer_state.error = ERROR_NONE;
	shell->lexer_state.error_msg = NULL;
}

void	cleanup_shell(t_shell *shell)
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
