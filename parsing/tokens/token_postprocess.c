/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_postprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/29 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_fd_number(const char *str)
{
	if (!str || ft_strlen(str) != 1)
		return (0);
	return (str[0] >= '0' && str[0] <= '2');
}

static void	merge_fd_redirect(t_token *fd_token, t_token *redir_token)
{
	char	*new_value;

	if (fd_token->value[0] == '2')
	{
		redir_token->type = TOKEN_REDIR_ERR;
		new_value = ft_strdup("2>");
	}
	else if (fd_token->value[0] == '1')
	{
		new_value = ft_strdup("1>");
	}
	else
	{
		new_value = ft_strdup("0<");
		if (redir_token->type == TOKEN_REDIR_OUT)
			redir_token->type = TOKEN_REDIR_IN;
	}
	if (new_value)
	{
		free(redir_token->value);
		redir_token->value = new_value;
	}
}

static void	handle_fd_redirect_merge(t_token **tokens, t_token **current,
		t_token *next_token, t_token **prev)
{
	merge_fd_redirect(*current, next_token);
	if (*prev)
		(*prev)->next = next_token;
	else
		*tokens = next_token;
	free((*current)->value);
	free(*current);
	*current = next_token;
}

t_token	*postprocess_fd_redirects(t_token *tokens)
{
	t_token	*current;
	t_token	*next_token;
	t_token	*prev;

	current = tokens;
	prev = NULL;
	while (current && current->next)
	{
		next_token = current->next;
		if (current->type == TOKEN_WORD && is_fd_number(current->value)
			&& (next_token->type == TOKEN_REDIR_OUT
				|| next_token->type == TOKEN_REDIR_IN))
			handle_fd_redirect_merge(&tokens, &current, next_token, &prev);
		else
		{
			prev = current;
			current = next_token;
		}
	}
	return (tokens);
}
