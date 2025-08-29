/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/08/17 14:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_pid_expansion_ext(char **result, int *i)
{
	char	*pid_str;
	char	*new_result;

	pid_str = ft_itoa(42);
	if (!pid_str)
		return (*result);
	new_result = ft_strjoin_free(*result, pid_str);
	free(pid_str);
	if (!new_result)
	{
		if (*result)
		{
			free(*result);
			*result = NULL;
		}
		return (NULL);
	}
	*result = new_result;
	*i += 2;
	return (*result);
}

static char	*process_var_value(char **result, char *var_value)
{
	char	*new_result;

	new_result = ft_strjoin_free(*result, var_value);
	free(var_value);
	if (!new_result)
	{
		if (*result)
		{
			free(*result);
			*result = NULL;
		}
		return (NULL);
	}
	*result = new_result;
	return (*result);
}

char	*handle_variable_expansion_ext(char *str, char **result, int *i,
		t_expansion_context *ctx)
{
	char	*var_name;
	char	*var_value;
	int		start;
	int		len;

	start = ++(*i);
	len = get_var_name_length(str, start);
	*i = start + len;
	var_name = ft_substr(str, start, len);
	if (!var_name)
		return (*result);
	var_value = get_var_value(var_name, ctx->env, ctx->last_status);
	if (var_value)
	{
		if (!process_var_value(result, var_value))
		{
			free(var_name);
			return (NULL);
		}
	}
	free(var_name);
	return (*result);
}
