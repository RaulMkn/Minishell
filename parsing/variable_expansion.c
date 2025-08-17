/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:56:53 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 13:25:05 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*process_expansion_loop(char *str, int len,
		t_expansion_context *ctx)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	while (i < len)
	{
		process_character_expansion(str, &result, &i, ctx);
	}
	return (result);
}

char	*expand_variables(char *str, char **env, int last_status, int len)
{
	char				*result;
	char				*quoted_result;
	t_expansion_context	ctx;

	if (!str || !env)
		return (NULL);
	len = ft_strlen(str);
	quoted_result = handle_quoted_expansion(str, env, last_status);
	if (quoted_result)
		return (quoted_result);
	ctx.env = env;
	ctx.last_status = last_status;
	ctx.inside_single_quotes = 0;
	result = process_expansion_loop(str, len, &ctx);
	if (result)
		return (result);
	return (ft_strdup(""));
}

void	expand_and_filter_tokens(t_token **tokens, t_shell *shell)
{
	t_token	*current;
	t_token	*prev;
	char	*expanded_value;

	current = *tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded_value = expand_variables(current->value, shell->envp,
					shell->last_status, 0);
			if (!expanded_value)
			{
				current = remove_empty_token(tokens, prev, current);
				continue ;
			}
			else
				update_token_value(current, expanded_value);
		}
		prev = current;
		current = current->next;
	}
}
