/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/14 17:25:54 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	if (argc != 1)
		return (1);
	(void)argv;
	init_shell(&shell, envp);
	setup_interactive_signals();
	shell_loop(&shell);
	cleanup_shell(&shell);
	rl_clear_history();
	return (shell.last_status);
}
