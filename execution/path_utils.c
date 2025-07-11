/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:45:08 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/11 20:11:48 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*join_paths(const char *dir, const char *cmd)
{
	char	*joined_path;
	size_t	len_dir;
	char	*slash;
	char	*final_path;

	slash = "";
	if (!dir || !cmd)
		return (NULL);
	len_dir = ft_strlen(dir);
	if (dir[len_dir - 1] != '/')
		slash = "/";
	joined_path = ft_strjoin(dir, slash);
	if (!joined_path)
		return (NULL);
	final_path = ft_strjoin(joined_path, cmd);
	free(joined_path);
	if (!final_path)
		return (NULL);
	return (final_path);
}

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

char	*find_command_path(char *cmd, char **envp)
{
	const char	*path_env = get_path_env(envp);
	char		**path_dirs;
	char		*full_path;
	int			i;

	if (!cmd || !*cmd || !envp)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	full_path = NULL;
	i = -1;
	while (path_dirs[++i])
	{
		full_path = join_paths(path_dirs[i], cmd);
		if (!full_path || (access(full_path, X_OK) == 0))
			break ;
		free(full_path);
		full_path = NULL;
	}
	return (free_split(path_dirs), full_path);
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

void	execute_command(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	if (!cmd || !*cmd)
		exit(126);
	args = ft_split(cmd, ' ');
	if (!args || !args[0])
	{
		free_split(args);
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	args = clean_arguments(args, &path, envp);
	execve(path, args, envp);
	perror(path);
	free_split(args);
	free(path);
	exit(errno);
}
