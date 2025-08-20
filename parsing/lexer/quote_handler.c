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

static int	has_matching_quote(const char *s, int start, char q)
{
	int	k;

	k = start;
	while (s[k])
	{
		if (s[k] == q)
			return (1);
		k++;
	}
	return (0);
}

static int	copy_single_quote(const char *str, char *res, int i, int *j)
{
	i++;
	while (str[i] && str[i] != '\'')
		res[(*j)++] = str[i++];
	if (str[i] == '\'')
		i++;
	return (i);
}

static int	copy_double_quote(const char *str, char *res, int i, int *j)
{
	i++;
	while (str[i] && str[i] != '\"')
		res[(*j)++] = str[i++];
	if (str[i] == '\"')
		i++;
	return (i);
}

static int	handle_quote(const char *str, char *res, int i, int *j)
{
	char	q;

	q = str[i];
	if (!has_matching_quote(str, i + 1, q))
	{
		res[(*j)++] = str[i++];
		return (i);
	}
	if (q == '\'')
		return (copy_single_quote(str, res, i, j));
	else
		return (copy_double_quote(str, res, i, j));
}

void	remove_quotes_copy(const char *str, char *res)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			i = handle_quote(str, res, i, &j);
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
}
