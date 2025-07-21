/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:48:24 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_redir_type	get_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_APPEND)
		return (REDIR_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (HEREDOC);
	return (REDIR_IN);
}

static t_redir	*create_redirection(t_token *token)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->type = get_redir_type(token->type);
	new_redir->file = ft_strdup(token->next->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->next = NULL;
	return (new_redir);
}

static t_redir	*add_new_redirection(t_redir *redir, t_token *token)
{
	t_redir	*new_redir;

	new_redir = create_redirection(token);
	if (!new_redir)
		return (NULL);
	new_redir->next = redir;
	return (new_redir);
}

static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

t_redir	*parse_redirections(t_token **tokens)
{
	t_redir	*redir;
	t_token	*token;

	redir = NULL;
	token = *tokens;
	while (token && is_redirection_token(token->type))
	{
		if (!token->next || token->next->type != TOKEN_WORD)
			return (NULL);
		redir = add_new_redirection(redir, token);
		if (!redir)
			return (NULL);
		token = token->next->next;
	}
	*tokens = token;
	return (redir);
}
