/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 15:01:57 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_buffer_token(t_token **tokens, char **buffer)
{
	if (*buffer && (*buffer)[0] != '\0')
	{
		create_word_token(tokens, buffer);
	}
}

static void	handle_word(t_token **tokens, char *word, t_shell *shell)
{
	t_token	*token;

	token = create_token(TOKEN_WORD, word);
	if (!token)
	{
		shell->lexer_state.error = ERROR_MEMORY;
		return ;
	}
	token_add_back(tokens, token);
}

void	handle_final_buffer(t_token **tokens, char **buffer, t_shell *shell)
{
	if (*buffer && (*buffer)[0] != '\0')
	{
		handle_word(tokens, *buffer, shell);
		free(*buffer);
		*buffer = NULL;
	}
}
