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

char	*handle_variable_expansion_ext(char *str, char **result, int *i,
		t_expansion_context *ctx)
{
	char	*var_name;
	char	*var_value;
	char	*new_result;
	int		start;
	int		len;
	int		j;

	j = *i;
	start = ++j;
	len = get_var_name_length(str, start);
	j = start + len;
	var_name = ft_substr(str, start, len);
	if (!var_name)
		return (*result);
	var_value = get_var_value(var_name, ctx->env, ctx->last_status);
	if (var_value)
	{
		new_result = ft_strjoin_free(*result, var_value);
		free(var_value);
		if (!new_result)
		{
			free(var_name);
			if (*result)
			{
				free(*result);
				*result = NULL;
			}
			return (NULL);
		}
		*result = new_result;
	}
	*i = j;
	free(var_name);
	return (*result);
}
