/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 13:43:19 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/08/17 13:49:49 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_regular_char(char **buffer, char c, size_t *i, t_shell *shell)
{
	if (((c >= 9 && c <= 13) || c == 32)
		&& shell->lexer_state.quote_state == QUOTE_NONE)
		return ;
	*buffer = ft_strjoin_char(*buffer, c);
	if (*buffer)
		(*i)++;
}

static void	handle_single_quote(char **buffer, size_t *i, t_shell *shell)
{
	*buffer = ft_strjoin_char(*buffer, '\'');
	if (!*buffer)
		return ;
	if (shell->lexer_state.quote_state == QUOTE_DOUBLE)
	{
		(*i)++;
		return ;
	}
	if (shell->lexer_state.quote_state == QUOTE_SINGLE)
		shell->lexer_state.quote_state = QUOTE_NONE;
	else if (shell->lexer_state.quote_state == QUOTE_NONE)
		shell->lexer_state.quote_state = QUOTE_SINGLE;
	(*i)++;
}

static void	handle_double_quote(char **buffer, size_t *i, t_shell *shell)
{
	*buffer = ft_strjoin_char(*buffer, '\"');
	if (!*buffer)
		return ;
	if (shell->lexer_state.quote_state == QUOTE_SINGLE)
	{
		(*i)++;
		return ;
	}
	if (shell->lexer_state.quote_state == QUOTE_DOUBLE)
		shell->lexer_state.quote_state = QUOTE_NONE;
	else if (shell->lexer_state.quote_state == QUOTE_NONE)
		shell->lexer_state.quote_state = QUOTE_DOUBLE;
	(*i)++;
}

void	handle_quote_char(char **buffer, char c, size_t *i, t_shell *shell)
{
	if (c == '\'')
		handle_single_quote(buffer, i, shell);
	else if (c == '\"')
		handle_double_quote(buffer, i, shell);
}
