/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 13:31:37 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_operator_syntax(char *s, size_t i)
{
	if (!s[i + 1] || !s[i + 2])
		return (1);
	if (s[i] == '<' && s[i + 1] == '>' && (!s[i + 2] || s[i + 2] == ' ' || s[i
				+ 2] == '\t' || s[i + 2] == '\n'))
		return (0);
	if ((s[i] == '>' || s[i] == '<') && (s[i + 1] == '>' || s[i + 1] == '<'))
		return (0);
	if (s[i] == '|' && s[i + 1] == '|')
		return (0);
	return (1);
}

static void	handle_operator_error(t_token **tokens, char **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	while (*tokens)
	{
		free((*tokens)->value);
		free(*tokens);
		*tokens = (*tokens)->next;
	}
}

static int	check_adjacent_operators(char *s, size_t i)
{
	if (!s[i])
		return (1);
	if (s[i] == '|' || s[i] == '<' || s[i] == '>')
	{
		if (s[i] == '<' && s[i + 1] == '>')
			return (0);
		if (s[i] == '|' && s[i + 1] == '|')
			return (0);
		if ((s[i] == '<' && (s[i + 1] == '>' || s[i + 1] == '|'))
			|| (s[i] == '>' && (s[i + 1] == '<' || s[i + 1] == '|')))
			return (0);
	}
	return (1);
}

static void	handle_op_error(t_tokenizer_ctx *ctx, int skip)
{
	set_error(&ctx->shell->lexer_state, ERROR_SYNTAX,
		"syntax error near unexpected token `newline'");
	handle_operator_error(ctx->tokens, ctx->buffer);
	*ctx->i += skip;
}

void	handle_operator(t_tokenizer_ctx *ctx)
{
	if (ctx->input[*ctx->i] == '<' && ctx->input[*ctx->i + 1] == '>')
		return (handle_op_error(ctx, 2));
	if (!check_adjacent_operators(ctx->input, *ctx->i))
		return (handle_op_error(ctx, 1));
	handle_buffer_token(ctx->tokens, ctx->buffer, ctx->shell);
	if ((ctx->input[*ctx->i] == '>' || ctx->input[*ctx->i] == '<')
		&& ctx->input[*ctx->i + 1] == ctx->input[*ctx->i]
		&& ctx->input[*ctx->i + 1] != ' ')
	{
		token_add_back(ctx->tokens, create_operator_token(ctx->input[*ctx->i],
				2));
		*ctx->i += 2;
	}
	else
	{
		token_add_back(ctx->tokens, create_operator_token(ctx->input[*ctx->i],
				1));
		(*ctx->i)++;
	}
}
