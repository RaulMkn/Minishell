/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/18 21:23:04 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../minishell.h"

static int	handle_valid_dotdot_path(char ***env, char *resolved_pwd,
		char *with_dotdot)
{
	char	*real_pwd;

	real_pwd = getcwd(NULL, 0);
	if (real_pwd)
	{
		set_env_variable(env, "PWD", real_pwd);
		free(real_pwd);
	}
	else
		set_env_variable(env, "PWD", resolved_pwd);
	free(resolved_pwd);
	free(with_dotdot);
	return (0);
}

static int	handle_cd_dotdot_complex(char ***env)
{
	char	*current_pwd;
	char	*with_dotdot;
	char	*resolved_pwd;

	current_pwd = get_env_value(*env, "PWD");
	if (!current_pwd)
		return (write(2, "minishell: cd: ..: No such file or directory\n", 46),
			1);
	if (current_pwd[ft_strlen(current_pwd) - 1] == '/')
		with_dotdot = ft_strjoin(current_pwd, "..");
	else
		with_dotdot = ft_strjoin(current_pwd, "/..");
	if (!with_dotdot)
		return (1);
	resolved_pwd = resolve_dotdots(with_dotdot);
	if (resolved_pwd && access(resolved_pwd, F_OK) == 0)
		return (handle_valid_dotdot_path(env, resolved_pwd, with_dotdot));
	set_env_variable(env, "PWD", with_dotdot);
	free(with_dotdot);
	if (resolved_pwd)
		free(resolved_pwd);
	write(2, "cd: error retrieving current directory: getcwd: "
		"cannot access parent directories: No such file or directory\n", 108);
	return (0);
}

int	handle_cd_dotdot(char ***env)
{
	int	result;

	result = handle_cd_dotdot_simple();
	if (result != -1)
		return (result);
	return (handle_cd_dotdot_complex(env));
}
