/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 03:27:25 by rmakende         ###   ########.fr       */
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
	(void)shell;
	if (!word || !*word)
		return ;
	
	// Simplemente crear el token con el word tal como está
	// La expansión de variables se manejará después en expand_and_filter_tokens
	token_add_back(tokens, create_token(TOKEN_WORD, ft_strdup(word)));
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

char	*ft_strjoin_char(char *str, char c)
{
	char	*new;
	int		len;

	len = 0;
	if (str)
		len = ft_strlen(str);
	new = malloc(sizeof(char) * (len + 2));
	if (!new)
	{
		free(str);
		return (NULL);
	}
	if (str)
	{
		copy_string_chars(new, str, len);
		free(str);
	}
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}
