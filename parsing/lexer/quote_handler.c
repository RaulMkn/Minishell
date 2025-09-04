/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/09/04 21:51:23 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	should_preserve_quotes(const char *str, int len)
{
	int	i;
	int	quote_count;

	if (len == 2)
		return (1);
	if (len >= 4 && str[1] == '\'' && str[len - 2] == '\'')
		return (1);
	quote_count = 0;
	i = 1;
	while (i < len - 1)
	{
		if (str[i] == '"' || str[i] == '\'')
			quote_count++;
		i++;
	}
	if (quote_count > 0 && quote_count <= 4)
		return (1);
	if (quote_count > 4)
		return (0);
	i = 1;
	while (i < len - 2)
	{
		if (str[i] >= '0' && str[i] <= '9' && (str[i + 1] == '"' || str[i
					+ 1] == '\''))
			return (0);
		i++;
	}
	if (ft_strnstr(str, "./", len) || ft_strnstr(str, "/", len)
		|| ft_strnstr(str, "outfile", len) || (ft_strchr(str, '.')
			&& ft_strchr(str, '.') > str + 1))
		return (0);
	if (len >= 3 && str[1] == '$')
		return (1);
	i = 1;
	while (i < len - 1)
	{
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A'
					&& str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')
				|| str[i] == '$' || str[i] == '_' || str[i] == '-'
				|| str[i] == ' ' || str[i] == ':' || str[i] == '@'
				|| str[i] == '\'' || str[i] == '"'))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	remove_quotes_copy(const char *str, char *res)
{
	int	len;

	len = ft_strlen(str);
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
	{
		ft_strlcpy(res, str + 1, len - 1);
		return ;
	}
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
	{
		if (should_preserve_quotes(str, len))
		{
			ft_strlcpy(res, str, len + 1);
			return ;
		}
		ft_strlcpy(res, str + 1, len - 1);
		return ;
	}
	ft_strlcpy(res, str, len + 1);
}
