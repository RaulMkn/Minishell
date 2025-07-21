/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:48:24 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_lexer_state(t_shell *shell)
{
	shell->lexer_state.quote_state = QUOTE_NONE;
	shell->lexer_state.error = ERROR_NONE;
	shell->lexer_state.error_msg = NULL;
}

static int	is_operator_char(char c, t_shell *shell)
{
	return ((c == '|' || c == '<' || c == '>')
		&& shell->lexer_state.quote_state == QUOTE_NONE);
}

static int	handle_unclosed_quotes(t_shell *shell, char **buffer)
{
	if (shell->lexer_state.quote_state != QUOTE_NONE)
	{
		set_error(&shell->lexer_state, ERROR_SYNTAX, "Comillas sin cerrar");
		free(*buffer);
		return (0);
	}
	return (1);
}

static void	process_character(t_token **tokens, char **buffer, size_t *i,
		char *input, t_shell *shell)
{
	if (input[*i] == '\'' || input[*i] == '\"')
		handle_quote_char(buffer, input[*i], i, shell);
	else if ((input[*i] >= 9 && input[*i] <= 13) || input[*i] == 32)
	{
		if (shell->lexer_state.quote_state == QUOTE_NONE)
			handle_whitespace(tokens, buffer, i, input);
		else
		{
			*buffer = ft_strjoin_char(*buffer, input[*i]);
			if (*buffer)
				(*i)++;
		}
	}
	else if (is_operator_char(input[*i], shell))
		handle_operator(tokens, buffer, i, input);
	else
		handle_regular_char(buffer, input[*i], i, shell);
}

t_token	*tokenize_input(char *in, t_shell *shell)
{
	t_token	*tokens;
	size_t	i;
	char	*buffer;

	tokens = NULL;
	i = 0;
	buffer = NULL;
	if (!in || !shell)
		return (NULL);
	init_lexer_state(shell);
	while (in[i])
		process_character(&tokens, &buffer, &i, in, shell);
	if (!handle_unclosed_quotes(shell, &buffer))
		return (NULL);
	if (buffer)
		handle_final_buffer(&tokens, &buffer, shell);
	return (tokens);
}
