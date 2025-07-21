/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:48:24 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_buffer_token(t_token **tokens, char **buffer)
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
	if (!new_token)
	{
		free(temp);
		free(*buffer);
		*buffer = NULL;
		return ;
	}
	token_add_back(tokens, new_token);
	free(temp);
	free(*buffer);
	*buffer = NULL;
}

static void	handle_word(t_token **tokens, char *word, t_shell *shell)
{
	char	*expanded;

	if (shell->lexer_state.quote_state == QUOTE_SINGLE)
	{
		token_add_back(tokens, create_token(TOKEN_WORD, word));
		return ;
	}
	expanded = expand_variables(word, shell->envp, shell->last_status);
	if (!expanded)
	{
		set_error(&shell->lexer_state, ERROR_MEMORY,
			"Error de memoria al expandir variables");
		return ;
	}
	token_add_back(tokens, create_token(TOKEN_WORD, expanded));
	free(expanded);
}

void	handle_final_buffer(t_token **tokens, char **buffer, t_shell *shell)
{
	if (!buffer || !*buffer)
		return ;
	if (**buffer != '\0')
		handle_word(tokens, *buffer, shell);
	free(*buffer);
	*buffer = NULL;
}
