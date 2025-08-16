/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/16 20:32:33 by rmakende         ###   ########.fr       */
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
	char	*cmd_name;
	t_token	*current;

	if (!word || !*word)
		return ;
	// Identificar si este token es parte del comando 'echo'
	cmd_name = NULL;
	current = *tokens;
	// Buscar el primer token (que sería el comando)
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			cmd_name = current->value;
			break ;
		}
		current = current->next;
	}

	if (cmd_name && ft_strcmp(cmd_name, "echo") == 0 && word[0] == '\''
		&& word[ft_strlen(word) - 1] == '\'')
	{

		token_add_back(tokens, create_token(TOKEN_WORD, ft_strdup(word)));
		return ;
	}
	// Comportamiento estándar para otros comandos o sin comillas simples
	if (shell->lexer_state.quote_state == QUOTE_SINGLE)
		return (token_add_back(tokens, create_token(TOKEN_WORD, word)));
	expanded = expand_variables(word, shell->envp, shell->last_status, 1);
	if (!expanded)
		expanded = ft_strdup("");
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
