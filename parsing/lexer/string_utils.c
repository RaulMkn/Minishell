/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/11 00:31:44 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	copy_string_chars(char *new, char *str, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
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

static size_t	handle_whitespaces(const char *input, size_t i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n'
			|| input[i] == '\v' || input[i] == '\f' || input[i] == '\r'))
		i++;
	return (i);
}

static void	create_word_token(t_token **tokens, char **buffer)
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
	char	*result;
	int		i;
	int		j;
	int		len;
	char	quote_char;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote_char = str[i];
			i++;
			while (i < len && str[i] != quote_char)
			{
				result[j++] = str[i++];
			}
			if (i < len && str[i] == quote_char)
				i++;
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}
