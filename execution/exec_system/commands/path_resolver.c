/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:45:08 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 15:01:57 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5) == envp[i])
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*search_in_path_dirs(char *cmd, char **path_dirs)
{
	char	*full_path;
	int		i;

	full_path = NULL;
	i = -1;
	while (path_dirs[++i])
	{
		full_path = ft_path_join(path_dirs[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			break ;
		free(full_path);
		full_path = NULL;
	}
	return (full_path);
}

char	*find_command_path(char *cmd, char **envp)
{
	const char	*path_env;
	char		**path_dirs;
	char		*result;

	if (!cmd || !*cmd || !envp)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	result = search_in_path_dirs(cmd, path_dirs);
	free_split(path_dirs);
	return (result);
}

char	**clean_arguments(char **args, char **path, char **envp)
{
	int	i;

	i = 0;
	while (args[i])
	{
		args[i] = ft_cleaner(args[i], "\"'");
		if (!args[i])
			return (NULL);
		i++;
	}
	*path = find_command_path(args[0], envp);
	if (!*path)
	{
		if (ft_strchr(args[0], '/'))
			perror(args[0]);
		else
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd("\n", 2);
		}
		free_split(args);
		exit(127);
	}
	return (args);
}
