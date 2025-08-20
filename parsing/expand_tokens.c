/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:52:55 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/08/19 15:01:57 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	char	*new_result;

	temp = ft_substr(str, *i, 1);
	if (!temp)
		return ;
	new_result = ft_strjoin_free(*result, temp);
	free(temp);
	if (!new_result)
	{
		if (*result)
		{
			free(*result);
			*result = NULL;
		}
		return ;
	}
	*result = new_result;
	(*i)++;
}
