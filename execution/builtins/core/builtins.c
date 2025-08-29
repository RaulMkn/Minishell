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

static int	handle_pwd_fallback(char **env)
{
	char	*env_pwd;

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

static void	update_pwd_if_needed(char **env, char *cwd)
{
	char	*env_pwd;

	env_pwd = get_env_value(env, "PWD");
	if (!env_pwd || ft_strcmp(env_pwd, cwd) != 0)
	{
		set_env_variable(&env, "PWD", cwd);
	}
}

int	builtin_pwd(char **env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		return (handle_pwd_fallback(env));
	}
	update_pwd_if_needed(env, cwd);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}
