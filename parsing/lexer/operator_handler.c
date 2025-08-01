/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 20:49:13 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_operator_syntax(char *s, size_t i)
{
	if (!s[i + 1] || !s[i + 2])
		return (1);
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

static int	is_valid_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	check_adjacent_operators(char *s, size_t i)
{
	if (!s[i])
		return (1);
	if (is_valid_operator_char(s[i]))
	{
		if (s[i] == '|' && s[i + 1] == '|')
			return (0);
		if ((s[i] == '<' && (s[i + 1] == '>' || s[i + 1] == '|'))
			|| (s[i] == '>' && (s[i + 1] == '<' || s[i + 1] == '|')))
			return (0);
	}
	return (1);
}

void	handle_operator(t_token **tokens, char **buffer, size_t *i, char *input)
{
	if (!check_adjacent_operators(input, *i))
	{
		handle_operator_error(tokens, buffer);
		return ;
	}
	handle_buffer_token(tokens, buffer);
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i]
		&& input[*i + 1] != ' ')
	{
		token_add_back(tokens, create_operator_token(input[*i], 2));
		(*i) += 2;
	}
	else
	{
		token_add_back(tokens, create_operator_token(input[*i], 1));
		(*i)++;
	}
}
