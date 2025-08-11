/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/11 19:45:45 by ruortiz-         ###   ########.fr       */
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

	if (!word || word[0] == '\0')
		return;
	if (shell->lexer_state.quote_state == QUOTE_SINGLE)
	{
		token_add_back(tokens, create_token(TOKEN_WORD, word));
		return ;
	}
	
	// Si la palabra contiene solo una variable que empieza con $
	if (word[0] == '$' && word[1])
	{
		expanded = expand_variables(word, shell->envp, shell->last_status);
		if (!expanded || expanded[0] == '\0')
		{
			// Variable vacía, NO crear token
			if (expanded)
				free(expanded);
			return ;
		}
		token_add_back(tokens, create_token(TOKEN_WORD, expanded));
		free(expanded);
		return ;
	}
	
	// Para palabras normales o mixtas
	expanded = expand_variables(word, shell->envp, shell->last_status);
	if (!expanded)
		return ;
	if (expanded[0] == '\0')
	{
		free(expanded);
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
