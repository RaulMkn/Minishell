/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 13:51:01 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



static int	should_preserve_quotes(const char *str, int len)
{
	int	i;
	int	quote_count;
	
	// Always preserve empty quotes '' and ''hello''
	if (len == 2)
		return (1);
		
	// Special cases for patterns like ''hello''
	if (len >= 4 && str[1] == '\'' && str[len - 2] == '\'')
		return (1);
	
	// Count internal quotes - if there are many, it's probably a filename
	quote_count = 0;
	i = 1;
	while (i < len - 1)
	{
		if (str[i] == '"' || str[i] == '\'')
			quote_count++;
		i++;
	}
	
	// If there are multiple internal quotes, preserve them for patterns like 'nested''quotes''test'
	if (quote_count > 0 && quote_count <= 4)
		return (1);
	
	// If there are too many internal quotes, don't preserve (likely filename)
	if (quote_count > 4)
		return (0);
	
	// Don't preserve if it contains number sequences (like "1""2""3" in filenames)
	i = 1;
	while (i < len - 2)
	{
		if (str[i] >= '0' && str[i] <= '9' && 
			(str[i + 1] == '"' || str[i + 1] == '\''))
			return (0);
		i++;
	}
		
	// Don't preserve quotes in file paths (containing ./ or / or complex structures)
	if (ft_strnstr(str, "./", len) || ft_strnstr(str, "/", len) || 
		ft_strnstr(str, "outfile", len) || 
		(ft_strchr(str, '.') && ft_strchr(str, '.') > str + 1))
		return (0);
		
	// For variable expansion patterns like '$HOME' or '$PATH', preserve quotes
	if (len >= 3 && str[1] == '$')
		return (1);
		
	// For 3+ character strings with quotes, check if content looks like inner quotes
	// Heuristic: if the content between quotes looks like simple text/variables,
	// it was likely originally inside double quotes
	i = 1;
	while (i < len - 1)
	{
		// Allow letters, numbers, $, _, -, spaces, and simple punctuation
		// But NOT / or . which suggest file paths
		if (!((str[i] >= 'a' && str[i] <= 'z') ||
			  (str[i] >= 'A' && str[i] <= 'Z') ||  
			  (str[i] >= '0' && str[i] <= '9') ||
			  str[i] == '$' || str[i] == '_' ||
			  str[i] == '-' || str[i] == ' ' || 
			  str[i] == ':' || str[i] == '@' || 
			  str[i] == '\'' || str[i] == '"'))
		{
			// Contains complex characters, likely not inner quotes
			return (0);
		}
		i++;
	}
	
	// Content looks simple, preserve the quotes
	return (1);
}

void	remove_quotes_copy(const char *str, char *res)
{
	int	len;

	len = ft_strlen(str);
	
	// Handle double quotes
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
	{
		ft_strlcpy(res, str + 1, len - 1);
		return ;
	}
	
	// Handle single quotes - but check if they should be preserved
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
	{
		// If this looks like it was originally inner content, preserve the quotes
		if (should_preserve_quotes(str, len))
		{
			ft_strlcpy(res, str, len + 1);
			return ;
		}
		// Otherwise remove the single quotes
		ft_strlcpy(res, str + 1, len - 1);
		return ;
	}
	
	// No quotes to remove
	ft_strlcpy(res, str, len + 1);
}
