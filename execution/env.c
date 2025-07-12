/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:04:24 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/12 16:22:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**clone_env(char **envp)
{
	int i;
	char **copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

int	get_env_index(char **env, const char *key)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	append_env(char ***env, const char *new_var)
{
	int		i;
	char	**new_env;

	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while ((*env)[i])
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i++] = ft_strdup(new_var);
	new_env[i] = NULL;
	//free(*env);
	*env = new_env;
	return (0);
}

int	replace_env(char **env, const char *key, const char *new_var)
{
	int index = get_env_index(env, key);
	if (index < 0)
		return (1);
	free(env[index]);
	env[index] = ft_strdup(new_var);
	return (0);
}

void	remove_env_entry(char ***env, int idx)
{
	int		i = 0;
	int		j = 0;
	char	**new_env;

	while ((*env)[i])
		i++;

	new_env = malloc(sizeof(char *) * i);
	if (!new_env)
		return ;

	i = 0;
	while ((*env)[i])
	{
		if (i != idx)
			new_env[j++] = (*env)[i];
		else
			free((*env)[i]);
		i++;
	}
	new_env[j] = NULL;
	free(*env);
	*env = new_env;
}