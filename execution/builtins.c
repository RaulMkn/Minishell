/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:03:17 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/14 22:12:29 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(const char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

int	run_builtin(char **argv, char ***env, char *line)
{
	if (!ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	else if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(argv, env));
	else if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd());
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

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

int	builtin_cd(char **argv, char ***env)
{
	(void)env;
	if (chdir(argv[1]) == -1)
	{
		perror("error:");
		return (1);
	}
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[1024];

	getcwd(cwd, sizeof(cwd));
	ft_printf("%s\n", cwd);
	return (0);
}

int	builtin_export(char **argv, char ***env)
{
	int		i;
	char	*eq;
	char	*key;
	int		j;

	i = 1;
	if (!argv[1])
	{
		j = 0;
		while ((*env)[j])
		{
			eq = ft_strchr((*env)[j], '=');
			if (eq)
			{
				write(1, "declare -x ", 11);
				write(1, (*env)[j], eq - (*env)[j]);
				write(1, "=\"", 2);
				write(1, eq + 1, ft_strlen(eq + 1));
				write(1, "\"\n", 2);
			}
			j++;
		}
		return (0);
	}
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!is_valid_identifier(argv[i]))
			write(2, "minishell: export: not a valid identifier\n", 42);
		else if (eq)
		{
			key = ft_substr(argv[i], 0, eq - argv[i]);
			if (get_env_index(*env, key) >= 0)
				replace_env(*env, key, argv[i]);
			else
				append_env(env, argv[i]);
			free(key);
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **argv, char ***env)
{
	int	i;
	int	idx;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(2, "minishell: unset: `", 20);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "`: not a valid identifier\n", 27);
		}
		else
		{
			idx = get_env_index(*env, argv[i]);
			if (idx != -1)
				remove_env_entry(env, idx);
		}
		i++;
	}
	return (0);
}
int	builtin_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

int	builtin_exit(char ***mini_env, char *line, char **argv)
{
	free_split(argv);
	free_split(*mini_env);
	free(line);
	rl_clear_history();
	exit(0);
}
