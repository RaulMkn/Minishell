/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/08/17 17:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	should_concatenate_tokens(t_token *current, t_token *next)
{
	if (current->type != TOKEN_WORD || next->type != TOKEN_WORD)
		return (0);
	if ((ft_strchr(current->value, '\'') && next->value[0] == '$')
		|| (ft_strchr(current->value, '"') && next->value[0] == '$'))
		return (1);
	return (0);
}

static int	merge_token_values(t_token *current, t_token *next)
{
	char	*new_value;

	new_value = ft_strjoin(current->value, next->value);
	if (!new_value)
		return (0);
	free(current->value);
	current->value = new_value;
	current->next = next->next;
	free(next->value);
	free(next);
	return (1);
}

t_token	*concatenate_consecutive_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current && current->next)
	{
		next = current->next;
		if (should_concatenate_tokens(current, next))
		{
			if (merge_token_values(current, next))
				continue ;
		}
		current = current->next;
	}
	return (tokens);
}

int	handle_pipe_token(t_token **curr_token, t_command *current_cmd)
{
	if (!current_cmd || !(*curr_token)->next)
		return (0);
	*curr_token = (*curr_token)->next;
	return (1);
}
