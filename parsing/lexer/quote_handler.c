/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 02:31:54 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_quotes(char c, t_lexer_state *state)
{
	if (c == '\'' && state->quote_state == QUOTE_NONE)
		state->quote_state = QUOTE_SINGLE;
	else if (c == '\'' && state->quote_state == QUOTE_SINGLE)
		state->quote_state = QUOTE_NONE;
	else if (c == '\"' && state->quote_state == QUOTE_NONE)
		state->quote_state = QUOTE_DOUBLE;
	else if (c == '\"' && state->quote_state == QUOTE_DOUBLE)
		state->quote_state = QUOTE_NONE;
	// Si estamos dentro de comillas dobles, las comillas simples no cambian el estado
	// Si estamos dentro de comillas simples, las comillas dobles no cambian el estado
}

void	handle_quote_char(char **buffer, char c, size_t *i, t_shell *shell)
{
	handle_quotes(c, &shell->lexer_state);
	*buffer = ft_strjoin_char(*buffer, c);
	(*i)++;
}

static int	is_whitespace_char(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

void	handle_regular_char(char **buffer, char c, size_t *i, t_shell *shell)
{
	if (is_whitespace_char(c) && shell->lexer_state.quote_state == QUOTE_NONE)
		return ;
	*buffer = ft_strjoin_char(*buffer, c);
	if (*buffer)
		(*i)++;
}

static int	has_internal_quotes(const char *str)
{
	int	i;
	int	len;

	len = ft_strlen(str);
	if (len < 3)
		return (0);
	
	// Buscar comillas internas (no al inicio ni al final)
	i = 1;
	while (i < len - 1)
	{
		if (str[i] == '"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

static int	find_matching_quote(const char *str, int start)
{
	char	quote_char;
	int		i;

	quote_char = str[start];
	i = start + 1;
	
	while (str[i])
	{
		if (str[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);  // No se encontró la comilla de cierre
}

void	remove_quotes_copy(const char *str, char *res)
{
	int		i;
	int		j;
	int		len;
	int		match_pos;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	
	// Caso 1: Cadena completamente entre comillas del mismo tipo Y sin comillas internas
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || 
					 (str[0] == '\'' && str[len - 1] == '\'')) && !has_internal_quotes(str))
	{
		// Eliminar solo las comillas externas
		i = 1;
		while (i < len - 1)
			res[j++] = str[i++];
	}
	else
	{
		// Caso 2: Eliminar solo comillas emparejadas
		while (str[i])
		{
			if (str[i] == '"' || str[i] == '\'')
			{
				match_pos = find_matching_quote(str, i);
				if (match_pos != -1)
				{
					// Hay un par de comillas - saltar ambas y copiar el contenido
					i++;  // Saltar la comilla de apertura
					while (i < match_pos)
						res[j++] = str[i++];
					i++;  // Saltar la comilla de cierre
				}
				else
				{
					// Comilla sin emparejar - copiarla tal como está
					res[j++] = str[i++];
				}
			}
			else
			{
				res[j++] = str[i++];
			}
		}
	}
	res[j] = '\0';
}

void	handle_escape_char(char **buffer, char *input, size_t *i, t_shell *shell)
{
	// En bash, el comportamiento de \ depende del contexto
	if (shell->lexer_state.quote_state == QUOTE_SINGLE)
	{
		// Dentro de comillas simples, \ es literal
		*buffer = ft_strjoin_char(*buffer, input[*i]);
		(*i)++;
	}
	else
	{
		// Fuera de comillas o en comillas dobles
		if (input[*i + 1] == '\\')
		{
			// \\ → \ (doble backslash se convierte en uno)
			*buffer = ft_strjoin_char(*buffer, '\\');
			*i += 2;
		}
		else if (input[*i + 1] == '"' && shell->lexer_state.quote_state == QUOTE_DOUBLE)
		{
			// \" dentro de comillas dobles → " literal
			*buffer = ft_strjoin_char(*buffer, '"');
			*i += 2;
		}
		else
		{
			// Otros casos: preservar el backslash tal como está
			*buffer = ft_strjoin_char(*buffer, input[*i]);
			(*i)++;
			// No procesar automáticamente el siguiente carácter aquí
		}
	}
}
