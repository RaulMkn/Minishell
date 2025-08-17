/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 13:44:16 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/08/17 13:44:30 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	append_char_to_result(char **result, char c)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	*result = build_result(*result, temp);
}

static char	*handle_quote_character(char *str, char **result, int *i,
		t_expansion_context *ctx)
{
	if (str[*i] == '\'')
	{
		ctx->inside_single_quotes = !ctx->inside_single_quotes;
		(*i)++;
		return (*result);
	}
	if (ctx->inside_single_quotes)
	{
		append_char_to_result(result, str[*i]);
		(*i)++;
		return (*result);
	}
	return (NULL);
}

static char	*handle_dollar_expansion(char *str, char **result, int *i,
		t_expansion_context *ctx)
{
	int	len;

	len = ft_strlen(str);
	if (*i < len - 1 && str[*i] == '$' && str[*i + 1] == '$')
		return (handle_pid_expansion_ext(result, i));
	if (*i < len - 1 && str[*i] == '$' && str[*i + 1] != ' '
		&& str[*i + 1] != '\0')
	{
		if (ctx->inside_single_quotes)
		{
			append_char_to_result(result, str[*i]);
			(*i)++;
			return (*result);
		}
		return (handle_variable_expansion_ext(str, result, i, ctx));
	}
	return (NULL);
}

char	*process_character_expansion(char *str, char **result, int *i,
		t_expansion_context *ctx)
{
	char	*quote_result;
	char	*dollar_result;
	int		len;

	len = ft_strlen(str);
	quote_result = handle_quote_character(str, result, i, ctx);
	if (quote_result != NULL)
		return (quote_result);
	dollar_result = handle_dollar_expansion(str, result, i, ctx);
	if (dollar_result != NULL)
		return (dollar_result);
	if (*i < len)
	{
		append_char_to_result(result, str[*i]);
		(*i)++;
	}
	return (*result);
}
