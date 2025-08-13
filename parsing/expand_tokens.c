/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:52:55 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/08/13 17:14:44 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*remove_empty_token(t_token **tokens, t_token *prev,
		t_token *current)
{
	t_token	*next;

	if (prev)
		prev->next = current->next;
	else
		*tokens = current->next;
	next = current->next;
	free(current->value);
	free(current);
	return (next);
}

void	update_token_value(t_token *current, char *expanded_value)
{
	free(current->value);
	current->value = expanded_value;
}

void	append_char(char **result, char *str, int *i)
{
	char	*temp;

	temp = ft_substr(str, *i, 1);
	*result = build_result(*result, temp);
	free(temp);
	(*i)++;
}
