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


static char	*resolve_dotdots(char *path)
{
	char	**parts;
	char	**resolved;
	int		i;
	int		j;
	char	*result;

	parts = ft_split(path, '/');
	if (!parts)
		return (ft_strdup(path));
	i = 0;
	while (parts[i])
		i++;
	resolved = malloc(sizeof(char *) * (i + 1));
	if (!resolved)
		return (free_split(parts), ft_strdup(path));
	i = 0;
	j = 0;
	while (parts[i])
	{
		if (ft_strcmp(parts[i], "..") == 0 && j > 0)
			j--;
		else if (ft_strcmp(parts[i], "..") != 0 && ft_strlen(parts[i]) > 0)
			resolved[j++] = parts[i];
		i++;
	}
	resolved[j] = NULL;
	if (j == 0)
		result = ft_strdup("/");
	else
	{
		result = ft_strdup("");
		i = 0;
		while (i < j)
		{
			result = ft_strjoin_free(result, "/");
			result = ft_strjoin_free(result, resolved[i]);
			i++;
		}
	}
	free(resolved);
	free_split(parts);
	if (!result || ft_strlen(result) == 0)
		return (free(result), ft_strdup("/"));
	return (result);
}

static int	handle_cd_dotdot_complex(char ***env)
{
	char	*current_pwd;
	char	*with_dotdot;
	char	*resolved_pwd;
	char	*real_pwd;

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
	{
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
	set_env_variable(env, "PWD", with_dotdot);
	free(with_dotdot);
	if (resolved_pwd)
		free(resolved_pwd);
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
