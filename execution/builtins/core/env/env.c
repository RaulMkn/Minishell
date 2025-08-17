/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:04:24 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/16 20:31:36 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	set_env_variable(char ***env, const char *key, const char *value)
{
	char	*key_eq;
	char	*new_var;
	char	*temp_key;

	if (!key || !value)
		return (1);
	key_eq = ft_strjoin(key, "=");
	if (!key_eq)
		return (1);
	new_var = ft_strjoin(key_eq, value);
	free(key_eq);
	if (!new_var)
		return (1);
	temp_key = ft_strdup(key);
	if (get_env_index(*env, key) >= 0)
	{
		replace_env(*env, temp_key, new_var);
	}
	else
	{
		append_env(env, new_var);
	}
	free(temp_key);
	free(new_var);
	return (0);
}

// ...existing code...

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
	char	**old_env;

	i = 0;
	old_env = *env;
	while (old_env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (old_env[i])
	{
		new_env[i] = old_env[i];
		i++;
	}
	new_env[i++] = ft_strdup(new_var);
	new_env[i] = NULL;
	free(old_env);
	*env = new_env;
	return (0);
}

int	replace_env(char **env, const char *key, const char *new_var)
{
	int	index;

	index = get_env_index(env, key);
	if (index < 0)
		return (1);
	free(env[index]);
	env[index] = ft_strdup(new_var);
	return (0);
}

void	remove_env_entry(char ***env, int idx)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
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
