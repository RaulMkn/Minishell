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

#include "../includes/minishell.h"

char	*handle_pid_expansion_ext(char **result, int *i)
{
	char	*pid_str;

	pid_str = ft_itoa(42);
	*result = build_result(*result, pid_str);
	free(pid_str);
	*i += 2;
	return (*result);
}

char	*handle_variable_expansion_ext(char *str, char **result, int *i,
		t_expansion_context *ctx)
{
	char	*var_name;
	char	*var_value;
	int		start;
	int		len;
	int		j;

	j = *i;
	start = ++j;
	len = get_var_name_length(str, start);
	j = start + len;
	var_name = ft_substr(str, start, len);
	var_value = get_var_value(var_name, ctx->env, ctx->last_status);
	if (var_value)
	{
		*result = build_result(*result, var_value);
		free(var_value);
	}
	*i = j;
	free(var_name);
	return (*result);
}
