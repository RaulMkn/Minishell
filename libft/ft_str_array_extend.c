/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_array_extend.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_str_array_extend(char **array, int new_size)
{
	char	**new_array;
	int		i;

	new_array = malloc(sizeof(char *) * new_size);
	if (!new_array)
		return (NULL);
	i = 0;
	if (array)
	{
		while (array[i] && i < new_size - 1)
		{
			new_array[i] = array[i];
			i++;
		}
		free(array);
	}
	while (i < new_size)
	{
		new_array[i] = NULL;
		i++;
	}
	return (new_array);
}
