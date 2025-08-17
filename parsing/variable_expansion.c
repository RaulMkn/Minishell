/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:56:53 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 02:05:50 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	char    *pid_str;
	int		i;

	result = NULL;
	i = 0;
	
	while (str[i])
	{
		// Solo rechazar wildcard *
		if (str[i] == '*')
		{
			free(result);
			return (ft_strdup(""));
		}
		
		// Manejar secuencias de $$ (PID)
		if (str[i] == '$' && str[i + 1] == '$')
		{
			pid_str = ft_itoa(getpid());
			result = build_result(result, pid_str);
			free(pid_str);
			i += 2;  // Avanzar más allá de los dos $
			continue;
		}
		
		// Manejar variables normales
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			var_value = process_variable_expansion(str, &i, env, last_status);
			result = build_result(result, var_value);
			if (var_value)
				free(var_value);
			continue;  // i ya se actualizó en process_variable_expansion
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
	char	*result;
	char    *pid_str;
	char    *var_value;
	int     i;
	int     inside_single_quotes = 0;

	if (!str || !env)
		return (NULL);
	
	len = ft_strlen(str);
	
	// Si la cadena entera está entre comillas simples, retornar el contenido literal
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		return (ft_strdup(str)); // Devolver con comillas para preservar literalidad
	
	// Las comillas dobles permiten expansión de variables
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
	{
		char *inner = ft_substr(str, 1, len - 2);
		char *expanded = expand_complex_variables(inner, env, last_status);
		free(inner);
		return (expanded);
	}
	
	// Para secuencias de $ fuera de comillas y manejo de comillas simples
	result = NULL;
	i = 0;
	while (i < len)
	{
		// Manejo de comillas simples - entrar/salir del modo literal
		if (str[i] == '\'')
		{
			inside_single_quotes = !inside_single_quotes;
			char temp[2] = {str[i], '\0'};
			result = build_result(result, temp);
			i++;
			continue;
		}
		
		// Si estamos dentro de comillas simples, todo es literal
		if (inside_single_quotes)
		{
			char temp[2] = {str[i], '\0'};
			result = build_result(result, temp);
			i++;
			continue;
		}
		
		// Detectar y expandir $$ (PID)
		if (i < len - 1 && str[i] == '$' && str[i + 1] == '$')
		{
			pid_str = ft_itoa(getpid());
			result = build_result(result, pid_str);
			free(pid_str);
			i += 2;
			continue;
		}
		
		// Detectar y expandir variables
		if (i < len - 1 && str[i] == '$' && str[i + 1] != ' ' && str[i + 1] != '\0')
		{
			int j = i;
			var_value = process_variable_expansion(str, &j, env, last_status);
			if (var_value)
			{
				result = build_result(result, var_value);
				free(var_value);
			}
			i = j;
			continue;
		}
		
		// Caracteres normales
		if (i < len)
		{
			char temp[2] = {str[i], '\0'};
			result = build_result(result, temp);
			i++;
		}
	}
	
	return (result ? result : ft_strdup(""));
}

// ...existing code...
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
            /* Si el token quedó totalmente envuelto por comillas simples,
               remove_quotes() se encarga en las fases siguientes según diseño. */
        }
        prev = current;
        current = current->next;
    }
}

