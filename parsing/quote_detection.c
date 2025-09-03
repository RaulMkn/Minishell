/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/09/03 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_delimiter_quoted(char *delimiter_token)
{
	int	len;

	if (!delimiter_token)
		return (0);
	len = ft_strlen(delimiter_token);
	if (len < 2)
		return (0);
	if ((delimiter_token[0] == '\'' && delimiter_token[len - 1] == '\'')
		|| (delimiter_token[0] == '"' && delimiter_token[len - 1] == '"'))
		return (1);
	if (delimiter_token[0] == '\\')
		return (1);
	return (0);
}
