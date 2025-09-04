/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 13:51:01 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_all_same_quotes(const char *str, int len)
{
	int	i;
	int	all_same_quotes;

	all_same_quotes = 1;
	i = 1;
	while (i < len - 1)
	{
		if (str[i] != str[0])
		{
			all_same_quotes = 0;
			break ;
		}
		i++;
	}
	return (all_same_quotes);
}

static int	has_matching_quotes(const char *str, int len)
{
	return (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')));
}

void	remove_quotes_copy(const char *str, char *res)
{
	int	len;
	int	all_same_quotes;

	len = ft_strlen(str);
	if (has_matching_quotes(str, len))
	{
		all_same_quotes = check_all_same_quotes(str, len);
		if (all_same_quotes)
		{
			ft_strlcpy(res, str, len + 1);
			return ;
		}
	}
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
		ft_strlcpy(res, str + 1, len - 1);
	else if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		ft_strlcpy(res, str + 1, len - 1);
	else
		ft_strlcpy(res, str, len + 1);
}
