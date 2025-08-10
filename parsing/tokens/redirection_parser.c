/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/07 19:02:52 by ruortiz-         ###   ########.fr       */
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
	new_redir->file = remove_quotes(token->next->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->next = NULL;
	return (new_redir);
}

static t_redir *add_new_redirection(t_redir *redir, t_token *token)
{
    t_redir *new_redir;
    t_redir *current;

    new_redir = create_redirection(token);
    if (!new_redir)
        return (NULL);
    if (!redir)
    {
        new_redir->next = NULL;
        return (new_redir);
    }
    else
    {
        current = redir;
        while (current->next)
            current = current->next;
        current->next = new_redir;
        new_redir->next = NULL;
        return (redir);
    }
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

t_redir	*parse_redirections_mixed(t_token **tokens)
{
	t_redir	*redir;
	t_token	*token;

	redir = NULL;
	token = *tokens;
	while (token && token->type != TOKEN_PIPE)
	{
		if (is_redirection_token(token->type))
		{
			if (!token->next || token->next->type != TOKEN_WORD)
				return (clear_redir_list(redir), NULL);
			redir = add_new_redirection(redir, token);
			if (!redir)
				return (NULL);
			token = token->next->next;
		}
		else
			token = token->next;
	}
	*tokens = token;
	return (redir);
}

void	clear_redir_list(t_redir *redir)
{
	t_redir	*current;
	t_redir	*next;

	current = redir;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}
