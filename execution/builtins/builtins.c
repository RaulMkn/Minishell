/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:03:17 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/12 22:34:25 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_cd(char **argv, char ***env)
{
	char	*home;

	if (argv[1] && argv[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	if (argv[1] == NULL || (argv[1][0] == '\0'))
	{
		home = get_env_value(*env, "HOME");
		if (!home)
			return (write(2, "minishell: cd: HOME not set\n", 29), 1);
		if (chdir(home) == -1)
			return (perror("minishell: cd"), 1);
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			perror("minishell: cd");
			return (1);
		}
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

static int	export_display_all(char **env)
{
	int		j;
	char	*eq;

	j = 0;
	while (env[j])
	{
		eq = ft_strchr(env[j], '=');
		if (eq)
		{
			write(1, "declare -x ", 11);
			write(1, env[j], eq - env[j]);
			write(1, "=\"", 2);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		j++;
	}
	return (0);
}

static int	export_process_args(char **argv, char ***env, int *error_occurred)
{
	int		i;
	char	*eq;
	char	*key;

	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!is_valid_identifier(argv[i]))
		{
			write(2, "minishell: export: not a valid identifier\n", 42);
			*error_occurred = 1;
		}
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
	return (*error_occurred);
}

int	builtin_export(char **argv, char ***env)
{
	int	error_occurred;

	error_occurred = 0;
	if (!argv[1])
		return (export_display_all(*env));
	return (export_process_args(argv, env, &error_occurred));
}
