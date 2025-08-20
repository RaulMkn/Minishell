/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 15:01:57 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	char	*cwd;
	char	*env_pwd;

	shell->cmd_list = NULL;
	shell->envp = ft_str_array_dup(envp);
	if (!shell->envp)
	{
		shell->envp = malloc(sizeof(char *));
		if (!shell->envp)
		{
			shell->last_status = 1;
			return ;
		}
		shell->envp[0] = NULL;
	}
	shell->last_status = 0;
	shell->lexer_state.quote_state = QUOTE_NONE;
	shell->lexer_state.error = ERROR_NONE;
	shell->lexer_state.error_msg = NULL;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		env_pwd = get_env_value(shell->envp, "PWD");
		if (!env_pwd || ft_strcmp(env_pwd, cwd) != 0)
		{
			if (set_env_variable(&shell->envp, "PWD", cwd) != 0)
			{
				free(cwd);
				return ;
			}
		}
		free(cwd);
	}
	env_pwd = get_env_value(shell->envp, "OLDPWD");
	if (!env_pwd)
	{
		if (set_env_variable(&shell->envp, "OLDPWD", "") != 0)
			return ;
	}
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
