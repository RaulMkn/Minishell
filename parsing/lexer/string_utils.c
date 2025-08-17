/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 03:16:10 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	copy_string_chars(char *new, char *str, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
}

size_t	handle_whitespaces(const char *input, size_t i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n'
			|| input[i] == '\v' || input[i] == '\f' || input[i] == '\r'))
		i++;
	return (i);
}

void	create_word_token(t_token **tokens, char **buffer)
{
	char	*temp;

	if (!(*buffer && **buffer != '\0'))
		return ;
	temp = ft_strdup(*buffer);
	if (!temp)
	{
		free(*buffer);
		*buffer = NULL;
		return ;
	}
	token_add_back(tokens, create_token(TOKEN_WORD, temp));
	free(temp);
	free(*buffer);
	*buffer = NULL;
}

void	handle_whitespace(t_token **tokens, char **buffer, size_t *i,
		char *input)
{
	create_word_token(tokens, buffer);
	*i = handle_whitespaces(input, *i);
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
