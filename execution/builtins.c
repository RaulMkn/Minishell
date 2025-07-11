/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:03:17 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/11 20:17:52 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"echo") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"));
}

void	run_builtin(char **argv)
{
	char cwd[1024];

	if (!ft_strcmp(argv[0], "echo"))
	{
		for (int i = 1; argv[i]; i++)
			ft_printf("%s ", argv[i]);
		ft_printf("\n");
	}
	else if (!ft_strcmp(argv[0], "pwd"))
	{
		getcwd(cwd, sizeof(cwd));
		ft_printf("%s\n", cwd);
	}
	else if (!ft_strcmp(argv[0], "exit"))
		exit(0);
	else
		ft_printf("Builtin '%s' no implementado aún\n", argv[0]);
}