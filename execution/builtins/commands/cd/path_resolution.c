/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/09/02 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../minishell.h"

static int	process_path_parts(char **parts, char **resolved)
{
	int	i;
	int	j;

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
	return (j);
}

static char	*build_resolved_path(char **resolved, int j)
{
	char	*result;
	int		i;

	if (j == 0)
		return (ft_strdup("/"));
	result = ft_strdup("");
	i = 0;
	while (i < j)
	{
		result = ft_strjoin_free(result, "/");
		result = ft_strjoin_free(result, resolved[i]);
		i++;
	}
	return (result);
}

char	*resolve_dotdots(char *path)
{
	char	**parts;
	char	**resolved;
	char	*result;
	int		i;
	int		j;

	parts = ft_split(path, '/');
	if (!parts)
		return (ft_strdup(path));
	i = 0;
	while (parts[i])
		i++;
	resolved = malloc(sizeof(char *) * (i + 1));
	if (!resolved)
		return (free_split(parts), ft_strdup(path));
	j = process_path_parts(parts, resolved);
	resolved[j] = NULL;
	result = build_resolved_path(resolved, j);
	free(resolved);
	free_split(parts);
	if (!result || ft_strlen(result) == 0)
		return (free(result), ft_strdup("/"));
	return (result);
}
