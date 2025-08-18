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

static char	*prepare_parent_dir(char *current_pwd)
{
	char	*parent_dir;
	char	*last_slash;

	parent_dir = ft_strdup(current_pwd);
	if (parent_dir)
	{
		last_slash = ft_strrchr(parent_dir, '/');
		if (last_slash && last_slash != parent_dir)
			*last_slash = '\0';
		else if (last_slash == parent_dir)
			parent_dir[1] = '\0';
	}
	return (parent_dir);
}

static void	search_accessible_parent(char **parent_dir)
{
	char	*last_slash;

	while (*parent_dir && access(*parent_dir, F_OK) == -1)
	{
		last_slash = ft_strrchr(*parent_dir, '/');
		if (last_slash && last_slash != *parent_dir)
			*last_slash = '\0';
		else if (last_slash == *parent_dir)
		{
			(*parent_dir)[1] = '\0';
			break ;
		}
		else
		{
			free(*parent_dir);
			*parent_dir = ft_strdup("/");
			break ;
		}
	}
}

static void	find_accessible_parent(char *current_pwd)
{
	char	*parent_dir;

	parent_dir = prepare_parent_dir(current_pwd);
	if (parent_dir)
	{
		search_accessible_parent(&parent_dir);
		if (parent_dir)
			(chdir(parent_dir), free(parent_dir));
	}
}

static int	handle_cd_dotdot_complex(char ***env)
{
	char	*current_pwd;
	char	*with_dotdot;

	current_pwd = get_env_value(*env, "PWD");
	if (!current_pwd)
		return (write(2, "minishell: cd: ..: No such file or directory\n", 46),
			1);
	if (current_pwd[ft_strlen(current_pwd) - 1] == '/')
		with_dotdot = ft_strjoin(current_pwd, "..");
	else
		with_dotdot = ft_strjoin(current_pwd, "/..");
	if (with_dotdot)
	{
		set_env_variable(env, "PWD", with_dotdot);
		free(with_dotdot);
	}
	find_accessible_parent(current_pwd);
	write(2,
		"cd: error retrieving current directory: getcwd: "
		"cannot access parent directories: No such file or "
		"directory\n",
		108);
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
