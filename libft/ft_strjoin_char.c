/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_char(char *str, char c)
{
	char	*new;
	int		len;
	int		i;

	len = 0;
	if (str)
		len = ft_strlen(str);
	new = malloc(sizeof(char) * (len + 2));
	if (!new)
	{
		free(str);
		return (NULL);
	}
	i = 0;
	if (str)
	{
		while (i < len)
		{
			new[i] = str[i];
			i++;
		}
		free(str);
	}
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}
