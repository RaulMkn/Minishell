/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:56:40 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:22:05 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_redirection_file(t_token *token)
{
	if (!token || !token->next || token->next->type != TOKEN_WORD)
		return (0);
	return (1);
}

static int	check_pipe_syntax(t_token *token)
{
	if (!token->next || token->next->type == TOKEN_PIPE)
		return (0);
	return (1);
}

static int	check_sequence_start_end(t_token *tokens)
{
	t_token	*curr;

	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE)
		return (0);
	curr = tokens;
	while (curr->next)
		curr = curr->next;
	if (curr->type == TOKEN_PIPE)
		return (0);
	return (1);
}

int	is_valid_operator_sequence(t_token *tokens)
{
	t_token	*curr;

	if (!check_sequence_start_end(tokens))
		return (0);
	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE && !check_pipe_syntax(curr))
			return (0);
		if ((curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_REDIR_OUT
				|| curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC)
			&& !check_redirection_file(curr))
			return (0);
		curr = curr->next;
	}
	return (1);
}
