/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 15:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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
	char	*temp_path;

	if (!ft_strcmp(argv[1], ".."))
		handle_dotdot_fallback(current_pwd, env);
	else if (argv[1][0] == '/')
		set_env_variable(env, "PWD", argv[1]);
	else if (!ft_strcmp(argv[1], "-"))
	{
		temp_path = get_env_value(*env, "OLDPWD");
		if (temp_path)
			set_env_variable(env, "PWD", temp_path);
	}
	else
		handle_relative_path_fallback(argv, current_pwd, env);
}

static int	handle_pwd_based_dotdot(char ***env)
{
	char	*current_pwd;
	char	*parent_dir;
	char	*last_slash;

	current_pwd = get_env_value(*env, "PWD");
	if (!current_pwd)
		return (write(2, "minishell: cd: ..: No such file or directory\n",
				46), 1);
	parent_dir = ft_strdup(current_pwd);
	last_slash = ft_strrchr(parent_dir, '/');
	if (last_slash && last_slash != parent_dir)
		*last_slash = '\0';
	else if (last_slash == parent_dir)
		parent_dir[1] = '\0';
	if (chdir(parent_dir) == -1)
	{
		write(2, "minishell: cd: ..: No such file or directory\n", 46);
		return (free(parent_dir), 1);
	}
	free(parent_dir);
	return (0);
}

int	handle_cd_dotdot(char ***env)
{
	char	*cwd_check;

	cwd_check = getcwd(NULL, 0);
	if (!cwd_check)
		return (handle_pwd_based_dotdot(env));
	else
	{
		free(cwd_check);
		if (chdir("..") == -1)
			return (perror("minishell: cd"), 1);
	}
	return (0);
}
