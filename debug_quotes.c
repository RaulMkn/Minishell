/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/09/01 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_quote_positions(const char *str, int len, int *start, int *end)
{
	char	quote_char;

	quote_char = str[0];
	if ((quote_char == '"' || quote_char == '\'') && str[len - 1] == quote_char)
	{
		*start = 1;
		*end = len - 1;
		printf("DEBUG: Removing outer quotes (%c)\n", quote_char);
	}
	else
	{
		*start = 0;
		*end = len;
		printf("DEBUG: No matching outer quotes\n");
	}
}

static void	remove_quotes_export_debug(const char *str, char *res)
{
	int		len;
	int		start;
	int		end;

	printf("DEBUG: Input string: '%s'\n", str);
	len = ft_strlen(str);
	if (len >= 2)
		set_quote_positions(str, len, &start, &end);
	else
	{
		start = 0;
		end = len;
		printf("DEBUG: String too short\n");
	}
	ft_strlcpy(res, str + start, end - start + 1);
	printf("DEBUG: Result: '%s'\n", res);
}

int	main(void)
{
	char	result[100];
	char	*test;

	test = "hola mundo '";
	printf("Testing: '%s'\n", test);
	remove_quotes_export_debug(test, result);
	return (0);
}
