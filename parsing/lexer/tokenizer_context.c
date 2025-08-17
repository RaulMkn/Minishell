/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_context.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:30:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/08/17 16:30:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_tokenizer_context(t_tokenizer_ctx *ctx, t_token **tokens,
		char **buffer, size_t *i)
{
	*tokens = NULL;
	*i = 0;
	*buffer = NULL;
	ctx->tokens = tokens;
	ctx->buffer = buffer;
	ctx->i = i;
}

void	set_tokenizer_input(t_tokenizer_ctx *ctx, char *input)
{
	ctx->input = input;
}

t_token	*finalize_tokenization(t_shell *shell, char **buffer,
		t_token **tokens)
{
	if (!handle_unclosed_quotes(shell, buffer))
		return (NULL);
	if (*buffer)
		handle_final_buffer(tokens, buffer, shell);
	return (*tokens);
}

int	handle_forbidden_sequence_error(char *input, size_t i, t_shell *shell,
		t_token **tokens)
{
	if (is_forbidden_sequence(input, i, shell))
	{
		if (shell->lexer_state.error == ERROR_NONE)
			set_error(&shell->lexer_state, ERROR_SYNTAX,
				"Operador no soportado");
		clear_tokens(tokens);
		return (1);
	}
	return (0);
}
