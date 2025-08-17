/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_core.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/08/17 15:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_quoted_expansion(char *str, char **env, int last_status)
{
	char	*inner;
	char	*expanded;
	int		len;

	len = ft_strlen(str);
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
	{
		inner = ft_substr(str, 1, len - 2);
		expanded = expand_complex_variables(inner, env, last_status);
		free(inner);
		return (expanded);
	}
	return (NULL);
}
