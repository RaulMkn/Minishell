/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path_join.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 15:01:57 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_path_join(const char *dir, const char *file)
{
	char	*joined_path;
	size_t	len_dir;
	char	*slash;
	char	*final_path;

	slash = "";
	if (!dir || !file)
		return (NULL);
	len_dir = ft_strlen(dir);
	if (len_dir > 0 && dir[len_dir - 1] != '/')
		slash = "/";
	joined_path = ft_strjoin(dir, slash);
	if (!joined_path)
		return (NULL);
	final_path = ft_strjoin(joined_path, file);
	free(joined_path);
	if (!final_path)
		return (NULL);
	return (final_path);
}
