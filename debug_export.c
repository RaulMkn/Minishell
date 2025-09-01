/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/09/01 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_quoted_value(char *res, int key_len, char *value_part,
	int value_len)
{
	char	quote_char;

	quote_char = value_part[0];
	printf("DEBUG: First char: '%c', Last char: '%c'\n",
		value_part[0], value_part[value_len - 1]);
	if ((quote_char == '"' || quote_char == '\'')
		&& value_part[value_len - 1] == quote_char)
	{
		printf("DEBUG: Removing outer quotes\n");
		ft_strlcpy(res + key_len, value_part + 1, value_len - 1);
	}
	else
	{
		printf("DEBUG: No matching quotes, keeping as is\n");
		ft_strlcpy(res + key_len, value_part, value_len + 1);
	}
}

static void	debug_remove_quotes_export_copy(const char *str, char *res)
{
	char	*eq_pos;
	char	*value_part;
	int		key_len;
	int		value_len;

	printf("DEBUG: Input to export function: '%s'\n", str);
	eq_pos = ft_strchr(str, '=');
	if (!eq_pos)
	{
		ft_strlcpy(res, str, ft_strlen(str) + 1);
		return ;
	}
	key_len = eq_pos - str + 1;
	value_part = eq_pos + 1;
	value_len = ft_strlen(value_part);
	printf("DEBUG: Value part: '%s' (length: %d)\n", value_part, value_len);
	ft_strlcpy(res, str, key_len + 1);
	if (value_len >= 2)
		process_quoted_value(res, key_len, value_part, value_len);
	else
		ft_strlcpy(res + key_len, value_part, value_len + 1);
	printf("DEBUG: Final result: '%s'\n", res);
}

int	main(void)
{
	char	result[100];
	char	*test1;
	char	*test2;

	test1 = "a=\"hola mundo '\"";
	test2 = "a=hola mundo '";
	printf("Test 1: '%s'\n", test1);
	debug_remove_quotes_export_copy(test1, result);
	printf("\n");
	printf("Test 2: '%s'\n", test2);
	debug_remove_quotes_export_copy(test2, result);
	return (0);
}
