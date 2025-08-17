/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:48:24 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	token_add_back(t_token **tokens, t_token *new)
{
	t_token	*temp;

	if (!tokens || !new)
		return ;
	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	temp = *tokens;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

static t_token_type	get_operator_type(char c, int len)
{
	if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '<' && len == 1)
		return (TOKEN_REDIR_IN);
	else if (c == '>' && len == 1)
		return (TOKEN_REDIR_OUT);
	else if (c == '>' && len == 2)
		return (TOKEN_APPEND);
	else if (c == '<' && len == 2)
		return (TOKEN_HEREDOC);
	return (TOKEN_WORD);
}

static void	set_operator_string(char *op, char c, int len)
{
	op[0] = c;
	if (len == 2)
		op[1] = c;
	else
		op[1] = '\0';
	op[2] = '\0';
}

t_token	*create_operator_token(char c, int len)
{
	char			op[3];
	t_token_type	type;

	set_operator_string(op, c, len);
	type = get_operator_type(c, len);
	return (create_token(type, op));
}
