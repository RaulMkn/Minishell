/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 21:06:39 by rmakende         ###   ########.fr       */
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

static void	process_character(t_tokenizer_ctx *ctx, t_shell *shell)
{
	if (ctx->input[*ctx->i] == '\'' || ctx->input[*ctx->i] == '\"')
		handle_quote_char(ctx->buffer, ctx->input[*ctx->i], ctx->i, shell);
	else if ((ctx->input[*ctx->i] >= 9 && ctx->input[*ctx->i] <= 13)
		|| ctx->input[*ctx->i] == 32)
	{
		if (shell->lexer_state.quote_state == QUOTE_NONE)
			handle_whitespace(ctx->tokens, ctx->buffer, ctx->i, ctx->input);
		else
		{
			*ctx->buffer = ft_strjoin_char(*ctx->buffer, ctx->input[*ctx->i]);
			if (*ctx->buffer)
				(*ctx->i)++;
		}
	}
	else if (is_operator_char(ctx->input[*ctx->i], shell))
		handle_operator(ctx->tokens, ctx->buffer, ctx->i, ctx->input);
	else
		handle_regular_char(ctx->buffer, ctx->input[*ctx->i], ctx->i, shell);
}

t_token	*tokenize_input(char *in, t_shell *shell)
{
	t_token			*tokens;
	size_t			i;
	char			*buffer;
	t_tokenizer_ctx	ctx;

	tokens = NULL;
	i = 0;
	buffer = NULL;
	if (!in || !shell)
		return (NULL);
	init_lexer_state(shell);
	ctx.tokens = &tokens;
	ctx.buffer = &buffer;
	ctx.i = &i;
	ctx.input = in;
	while (in[i])
		process_character(&ctx, shell);
	if (!handle_unclosed_quotes(shell, &buffer))
		return (NULL);
	if (buffer)
		handle_final_buffer(&tokens, &buffer, shell);
	return (tokens);
}
