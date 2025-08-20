/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 14:02:41 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

static t_token	*process_input_loop(t_tokenizer_ctx *ctx, t_shell *shell,
		char **buffer, t_token **tokens)
{
	while (ctx->input[*ctx->i])
	{
		if (handle_forbidden_sequence_error(ctx->input, *ctx->i, shell,
				tokens))
		{
			if (*buffer)
				free(*buffer);
			return (NULL);
		}
		process_character(ctx, shell);
	}
	return (finalize_tokenization(shell, buffer, tokens));
}

t_token	*tokenize_input(char *input, t_shell *shell)
{
	t_token			*tokens;
	size_t			i;
	char			*buffer;
	t_tokenizer_ctx	ctx;

	if (!input || !shell)
		return (NULL);
	init_lexer_state(shell);
	init_tokenizer_context(&ctx, &tokens, &buffer, &i);
	set_tokenizer_input(&ctx, input);
	return (process_input_loop(&ctx, shell, &buffer, &tokens));
}
