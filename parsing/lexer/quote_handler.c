/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 02:08:19 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* static void	handle_quotes(char c, t_lexer_state *state)
{
	if (c == '\'' && state->quote_state == QUOTE_NONE)
		state->quote_state = QUOTE_SINGLE;
	else if (c == '\'' && state->quote_state == QUOTE_SINGLE)
		state->quote_state = QUOTE_NONE;
	else if (c == '\"' && state->quote_state == QUOTE_NONE)
		state->quote_state = QUOTE_DOUBLE;
	else if (c == '\"' && state->quote_state == QUOTE_DOUBLE)
		state->quote_state = QUOTE_NONE;
} */
// ...existing code...
// ...existing code...
// ...existing code...
void	handle_quote_char(char **buffer, char c, size_t *i, t_shell *shell)
{
    /* Comilla simple */
    if (c == '\'')
    {
        /* Si estamos dentro de double quotes, la comilla simple es literal */
        if (shell->lexer_state.quote_state == QUOTE_DOUBLE)
        {
            *buffer = ft_strjoin_char(*buffer, c);
            if (*buffer)
                (*i)++;
            return ;
        }
        /* Preservar la comilla simple en el buffer (apertura o cierre) */
        *buffer = ft_strjoin_char(*buffer, c);
        if (*buffer)
        {
            if (shell->lexer_state.quote_state == QUOTE_SINGLE)
                shell->lexer_state.quote_state = QUOTE_NONE;
            else if (shell->lexer_state.quote_state == QUOTE_NONE)
                shell->lexer_state.quote_state = QUOTE_SINGLE;
            (*i)++;
        }
        return ;
    }

    /* Comilla doble */
    if (c == '\"')
    {
        /* Si estamos dentro de single quotes, la comilla doble es literal */
        if (shell->lexer_state.quote_state == QUOTE_SINGLE)
        {
            *buffer = ft_strjoin_char(*buffer, c);
            if (*buffer)
                (*i)++;
            return ;
        }
        /* Preservar la comilla doble en el buffer (apertura o cierre) */
        *buffer = ft_strjoin_char(*buffer, c);
        if (*buffer)
        {
            if (shell->lexer_state.quote_state == QUOTE_DOUBLE)
                shell->lexer_state.quote_state = QUOTE_NONE;
            else if (shell->lexer_state.quote_state == QUOTE_NONE)
                shell->lexer_state.quote_state = QUOTE_DOUBLE;
            (*i)++;
        }
        return ;
    }
    /* ...existing code continues... */
}
// ...existing code...
// ...existing code...

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
    static int	has_matching_quote(const char *s, int start, char q)
    {
        int k = start;
        while (s[k])
        {
            if (s[k] == q)
                return (1);
            k++;
        }
        return (0);
    }
	// ...existing code...
	void	remove_quotes_copy(const char *str, char *res)
	{
		int		i;
		int		j;
		char	q;
	
		i = 0;
		j = 0;
		while (str[i])
		{
			if (str[i] == '"' || str[i] == '\'')
			{
				q = str[i];
				/* Si NO existe pareja más adelante, copiar la comilla como literal */
				if (!has_matching_quote(str, i + 1, q))
				{
					res[j++] = str[i++];
					continue ;
				}
				/* Si hay pareja, comportamiento distinto según tipo de comilla */
				if (q == '\'')
				{
					/* Mantener las comillas simples como delimitadores en la copia
					   para que la fase de expansión pueda detectarlas y evitar
					   expandir variables dentro. */
					res[j++] = str[i++]; /* copia la comilla de apertura ' */
					while (str[i] && str[i] != q)
						res[j++] = str[i++];
					if (str[i] == q)
						res[j++] = str[i++]; /* copia la comilla de cierre ' */
				}
				else /* q == '"' */
				{
					/* Para comillas dobles: omitir ambas comillas y copiar el interior */
					i++;
					while (str[i] && str[i] != q)
						res[j++] = str[i++];
					if (str[i] == q)
						i++;
				}
			}
			else
				res[j++] = str[i++];
		}
		res[j] = '\0';
	}
	// ...existing code...