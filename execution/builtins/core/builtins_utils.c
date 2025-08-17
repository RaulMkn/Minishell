/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 18:51:40 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 13:26:14 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	is_builtin(const char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

int	is_parent_builtin(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"export") || !ft_strcmp(cmd, "unset"));
}

int	run_builtin(char **argv, char ***env, char *line)
{
	if (!ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	else if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(argv, env));
	else if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd(*env));
	else if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(argv, env));
	else if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(argv, env));
	else if (!ft_strcmp(argv[0], "env"))
		return (builtin_env(*env));
	else if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(env, line, argv));
	return (1);
}

int	is_n_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
