/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 01:26:41 by rmakende         ###   ########.fr       */
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
static int	is_forbidden_sequence(char *input, size_t i, t_shell *shell)
{
    // Si el carácter está dentro de comillas, no considerarlo forbidden
    int j;
    int state; // 0 = none, 1 = single, 2 = double

    (void)shell;
    if (!input || i == 0)
        return (0);
    state = QUOTE_NONE;
    j = 0;
    while (j < (int)i)
    {
        if (input[j] == '\'' && state != QUOTE_DOUBLE)
            state = (state == QUOTE_SINGLE) ? QUOTE_NONE : QUOTE_SINGLE;
        else if (input[j] == '\"' && state != QUOTE_SINGLE)
            state = (state == QUOTE_DOUBLE) ? QUOTE_NONE : QUOTE_DOUBLE;
        j++;
    }
    if (state != QUOTE_NONE)
        return (0); // dentro de comillas -> no prohibir nada

    // Agregar detección para "<>" (fuera de comillas)
    if (input[i] == '<' && input[i + 1] == '>')
    {
        set_error(&shell->lexer_state, ERROR_SYNTAX,
            "syntax error near unexpected token `newline'");
        return (1);
    }
    if (input[i] == '&' && input[i + 1] == '&')
        return (1);
    if (input[i] == '|' && input[i + 1] == '|')
        return (1);
    // NO marcar '*' como forbidden aquí: el expand/filter debe manejarlo más tarde
    return (0);
}
// ...existing code...

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

t_token	*tokenize_input(char *input, t_shell *shell)
{
	t_token			*tokens;
	size_t			i;
	char			*buffer;
	t_tokenizer_ctx	ctx;

	tokens = NULL;
	i = 0;
	buffer = NULL;
	if (!input || !shell)
		return (NULL);
	init_lexer_state(shell);
	ctx.tokens = &tokens;
	ctx.buffer = &buffer;
	ctx.i = &i;
	ctx.input = input;
	while (input[i])
	{
		// Detectar secuencias prohibidas
		if (is_forbidden_sequence(input, i, shell))
		{
			if (shell->lexer_state.error == ERROR_NONE)
				set_error(&shell->lexer_state, ERROR_SYNTAX,
					"Operador no soportado");
			if (buffer)
				free(buffer);
			clear_tokens(&tokens);
			return (NULL);
		}
		process_character(&ctx, shell);
	}
	if (!handle_unclosed_quotes(shell, &buffer))
		return (NULL);
	if (buffer)
		handle_final_buffer(&tokens, &buffer, shell);
	return (tokens);
}
