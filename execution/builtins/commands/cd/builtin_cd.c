/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 18:56:16 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../minishell.h"

static int	handle_cd_home(char ***env)
{
	char	*home;

	home = get_env_value(*env, "HOME");
	if (!home)
		return (write(2, "minishell: cd: HOME not set\n", 29), 1);
	if (chdir(home) == -1)
		return (perror("minishell: cd"), 1);
	return (0);
}

static int	cd_change_directory(char **argv, char ***env)
{
	if (argv[1] == NULL || (argv[1][0] == '\0'))
		return (handle_cd_home(env));
	else if (!ft_strcmp(argv[1], "-"))
		return (write(2, "minishell: cd: option not supported\n", 37), 1);
	else if (!ft_strcmp(argv[1], ".."))
		return (handle_cd_dotdot(env));
	else
	{
		if (chdir(argv[1]) == -1)
			return (perror("minishell: cd"), 1);
	}
	return (0);
}

static void	update_pwd_after_cd(char **argv, char *current_pwd, char ***env)
{
	char	*new_pwd;
	char	*home;

	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		set_env_variable(env, "PWD", new_pwd);
		free(new_pwd);
	}
	else if (argv[1] && current_pwd)
		update_pwd_fallback(argv, current_pwd, env);
	else if (!argv[1])
	{
		home = get_env_value(*env, "HOME");
		if (home)
			set_env_variable(env, "PWD", home);
	}
}

void	set_env_variable_and_free(char ***env, const char *key,
		char *value)
{
	set_env_variable(env, key, value);
	free(value);
}
int	builtin_cd(char **argv, char ***env)
{
	char	*current_pwd;
	char	*current_pwd_copy;

	if (argv[1] && argv[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	current_pwd = get_env_value(*env, "PWD");
	if (!current_pwd)
	{
		current_pwd = getcwd(NULL, 0);
		if (current_pwd)
		{
			set_env_variable_and_free(env, "PWD", current_pwd);
			current_pwd = get_env_value(*env, "PWD");
		}
	}
	if (cd_change_directory(argv, env) != 0)
		return (1);
	if (current_pwd)
	{
		current_pwd_copy = ft_strdup(current_pwd);
		if (current_pwd_copy)
			set_env_variable_and_free(env, "OLDPWD", current_pwd_copy);
	}
	update_pwd_after_cd(argv, current_pwd, env);
	return (0);
}
