/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/09/01 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	remove_quotes_export_copy(const char *str, char *res)
{
	int	len;

	len = ft_strlen(str);
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
		ft_strlcpy(res, str + 1, len - 1);
	else if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		ft_strlcpy(res, str + 1, len - 1);
	else
		ft_strlcpy(res, str, len + 1);
}

char	*remove_quotes_export(const char *str)
{
	char	*res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	remove_quotes_export_copy(str, res);
	return (res);
}
