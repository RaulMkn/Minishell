/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 15:18:45 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

size_t	handle_whitespaces(const char *input, size_t i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n'
			|| input[i] == '\v' || input[i] == '\f' || input[i] == '\r'))
		i++;
	return (i);
}

void	create_word_token(t_token **tokens, char **buffer, t_shell *shell)
{
	char	*temp;
	t_token	*new_token;

	if (!(*buffer && **buffer != '\0'))
		return ;
	temp = ft_strdup(*buffer);
	if (!temp)
	{
		free(*buffer);
		*buffer = NULL;
		return ;
	}
	new_token = create_token(TOKEN_WORD, temp);
	if (new_token)
	{
		new_token->was_quoted = shell->lexer_state.current_token_quoted;
		shell->lexer_state.current_token_quoted = 0;
	}
	token_add_back(tokens, new_token);
	free(temp);
	free(*buffer);
	*buffer = NULL;
}

void	handle_whitespace(t_tokenizer_ctx *ctx)
{
	create_word_token(ctx->tokens, ctx->buffer, ctx->shell);
	*ctx->i = handle_whitespaces(ctx->input, *ctx->i);
}

char	*remove_quotes(const char *str)
{
	char	*res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	remove_quotes_copy(str, res);
	return (res);
}
