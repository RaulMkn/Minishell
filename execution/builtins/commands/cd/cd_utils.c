/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:21:38 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/18 21:23:21 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../minishell.h"

static void	handle_dotdot_fallback(char *current_pwd, char ***env)
{
	char	*parent;
	char	*last_slash;

	parent = ft_strdup(current_pwd);
	last_slash = ft_strrchr(parent, '/');
	if (last_slash && last_slash != parent)
		*last_slash = '\0';
	else if (last_slash == parent)
		parent[1] = '\0';
	set_env_variable(env, "PWD", parent);
	free(parent);
}

static void	handle_relative_path_fallback(char **argv, char *current_pwd,
		char ***env)
{
	char	*temp_path;
	char	*with_slash;

	if (current_pwd[ft_strlen(current_pwd) - 1] == '/')
		temp_path = ft_strjoin(current_pwd, argv[1]);
	else
	{
		with_slash = ft_strjoin(current_pwd, "/");
		temp_path = ft_strjoin(with_slash, argv[1]);
		free(with_slash);
	}
	if (temp_path)
	{
		set_env_variable(env, "PWD", temp_path);
		free(temp_path);
	}
}

void	update_pwd_fallback(char **argv, char *current_pwd, char ***env)
{
	if (!ft_strcmp(argv[1], ".."))
		handle_dotdot_fallback(current_pwd, env);
	else if (argv[1][0] == '/')
		set_env_variable(env, "PWD", argv[1]);
	else
		handle_relative_path_fallback(argv, current_pwd, env);
}

int	handle_cd_dotdot_simple(void)
{
	char	*cwd_check;

	cwd_check = getcwd(NULL, 0);
	if (cwd_check)
	{
		free(cwd_check);
		if (chdir("..") == -1)
			return (perror("minishell: cd"), 1);
		return (0);
	}
	return (-1);
}
