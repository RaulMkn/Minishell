/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/12 20:44:15 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_quotes(char c, t_lexer_state *state)
{
	if (c == '\'' && state->quote_state == QUOTE_NONE)
		state->quote_state = QUOTE_SINGLE;
	else if (c == '\'' && state->quote_state == QUOTE_SINGLE)
		state->quote_state = QUOTE_NONE;
	else if (c == '\"' && state->quote_state == QUOTE_NONE)
		state->quote_state = QUOTE_DOUBLE;
	else if (c == '\"' && state->quote_state == QUOTE_DOUBLE)
		state->quote_state = QUOTE_NONE;
}

void	handle_quote_char(char **buffer, char c, size_t *i, t_shell *shell)
{
	handle_quotes(c, &shell->lexer_state);
	*buffer = ft_strjoin_char(*buffer, c);
	(*i)++;
}

static int	is_whitespace_char(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

void	handle_regular_char(char **buffer, char c, size_t *i, t_shell *shell)
{
	if (is_whitespace_char(c) && shell->lexer_state.quote_state == QUOTE_NONE)
		return ;
	*buffer = ft_strjoin_char(*buffer, c);
	if (*buffer)
		(*i)++;
}

void	remove_quotes_copy(const char *str, char *res)
{
	int		i;
	int		j;
	char	q;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			q = str[i++];
			while (str[i] && str[i] != q)
				res[j++] = str[i++];
			if (str[i] == q)
				i++;
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
}
