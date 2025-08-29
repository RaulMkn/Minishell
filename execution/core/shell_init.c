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

static void	init_shell_shlvl(t_shell *shell)
{
	char	*shlvl_str;
	int		shlvl;
	char	*shlvl_new;
	int		flag;

	shlvl_str = get_env_value(shell->envp, "MINISHELL_SHLVL");
	if (shlvl_str)
	{
		shlvl = ft_atoi(shlvl_str, &flag);
		if (flag != 0)
			shlvl = 1;
		else
			shlvl++;
	}
	else
		shlvl = 1;
	shlvl_new = ft_itoa(shlvl);
	if (shlvl_new)
	{
		set_env_variable(&shell->envp, "SHLVL", shlvl_new);
		set_env_variable(&shell->envp, "MINISHELL_SHLVL", shlvl_new);
		free(shlvl_new);
	}
}

static void	init_shell_pwd(t_shell *shell)
{
	char	*cwd;
	char	*env_pwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		env_pwd = get_env_value(shell->envp, "PWD");
		if (!env_pwd || ft_strcmp(env_pwd, cwd) != 0)
			set_env_variable(&shell->envp, "PWD", cwd);
		free(cwd);
	}
}

void	init_shell(t_shell *shell, char **envp)
{
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
	init_shell_shlvl(shell);
	init_shell_pwd(shell);
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->envp)
		free_split(shell->envp);
	if (shell->cmd_list)
		clear_command(shell->cmd_list);
	if (shell->lexer_state.error_msg)
		free(shell->lexer_state.error_msg);
}
