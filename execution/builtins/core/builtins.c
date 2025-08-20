/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:03:17 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 13:21:52 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	builtin_pwd(char **env)
{
	char	*cwd;
	char	*env_pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		env_pwd = get_env_value(env, "PWD");
		if (env_pwd)
		{
			ft_printf("%s\n", env_pwd);
			return (0);
		}
		else
		{
			write(2, "pwd: error retrieving current directory: getcwd: "
				"cannot access parent directories: No such file or "
				"directory\n", 108);
			return (1);
		}
	}
	env_pwd = get_env_value(env, "PWD");
	if (!env_pwd || ft_strcmp(env_pwd, cwd) != 0)
	{
		set_env_variable(&env, "PWD", cwd);
	}
	ft_printf("%s\n", cwd);
	free(cwd);
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
