/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:56:53 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/13 17:14:58 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_simple_variable(char *str, char **env, int last_status)
{
	char	*var_name;
	char	*var_value;
	int		len;

	if (str[0] != '$' || !str[1])
		return (ft_strdup(str));
	len = get_var_name_length(str, 1);
	if (len == 0 || str[1 + len] != '\0')
		return (NULL);
	var_name = ft_substr(str, 1, len);
	var_value = get_var_value(var_name, env, last_status);
	free(var_name);
	if (var_value && var_value[0] == '\0')
	{
		free(var_value);
		return (NULL);
	}
	return (var_value);
}

static char	*process_variable_expansion(char *str, int *i, char **env,
		int last_status)
{
	char	*var_name;
	char	*var_value;
	int		start;
	int		len;

	start = ++(*i);
	len = get_var_name_length(str, start);
	*i = start + len;
	var_name = ft_substr(str, start, len);
	var_value = get_var_value(var_name, env, last_status);
	free(var_name);
	return (var_value);
}

static char	*expand_complex_variables(char *str, char **env, int last_status)
{
	char	*result;
	char	*var_value;
	int		i;

	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' '
			&& str[i + 1] != '"')
		{
			var_value = process_variable_expansion(str, &i, env, last_status);
			result = build_result(result, var_value);
			free(var_value);
		}
		else
			append_char(&result, str, &i);
	}
	if (result)
		return (result);
	return (ft_strdup(""));
}

char	*expand_variables(char *str, char **env, int last_status, int len)
{
	char	*simple_result;
	char	*inner;
	char	*expanded;
	char	*result;
	char	*final;

	if (!str || !env)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		return (ft_strdup(str));
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
	{
		inner = ft_substr(str, 1, len - 2);
		expanded = expand_complex_variables(inner, env, last_status);
		result = ft_strjoin("\"", expanded);
		final = ft_strjoin(result, "\"");
		return (free(inner), free(expanded), free(result), final);
	}
	simple_result = expand_simple_variable(str, env, last_status);
	if (simple_result)
		return (simple_result);
	return (expand_complex_variables(str, env, last_status));
}

void	expand_and_filter_tokens(t_token **tokens, t_shell *shell)
{
	t_token	*current;
	t_token	*prev;
	char	*expanded_value;
	int		len;

	current = *tokens;
	prev = NULL;
	len = 0;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded_value = expand_variables(current->value, shell->envp,
					shell->last_status, len);
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
