/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_complex.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/08/17 14:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_pid_expansion(char **result, int *i)
{
	char	*pid_str;

	pid_str = ft_itoa(42);
	*result = build_result(*result, pid_str);
	free(pid_str);
	*i += 2;
	return (*result);
}

static char	*process_var_expansion(char *str, char **result, int *i,
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
	var_value = get_var_value(var_name, ctx->env, ctx->last_status);
	if (var_value)
		*result = build_result(*result, var_value);
	free(var_name);
	if (var_value)
		free(var_value);
	return (*result);
}

static char	*process_complex_loop(char *str, t_expansion_context *ctx)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '$')
		{
			process_pid_expansion(&result, &i);
			continue ;
		}
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			process_var_expansion(str, &result, &i, ctx);
			continue ;
		}
		else
			append_char(&result, str, &i);
	}
	return (result);
}

char	*expand_complex_variables(char *str, char **env, int last_status)
{
	char				*result;
	t_expansion_context	ctx;

	ctx.env = env;
	ctx.last_status = last_status;
	result = process_complex_loop(str, &ctx);
	if (result)
		return (result);
	return (ft_strdup(""));
}
