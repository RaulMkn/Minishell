/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/11 19:45:45 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**extend_argv(char **argv, int size)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (size + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < size - 1)
	{
		new_argv[i] = argv[i];
		i++;
	}
	free(argv);
	return (new_argv);
}

static int	add_cleaned_arg(char **argv, t_token *token, int argc)
{
	char	*cleaned;

	if (!token || !token->value || token->value[0] == '\0')
		return (-1);  // Saltar token vacío
	cleaned = remove_quotes(token->value);
	if (!cleaned)
		return (0);  // Error de memoria
	if (cleaned[0] == '\0')
	{
		// Argumento completamente vacío, saltarlo
		free(cleaned);
		return (-1);
	}
	argv[argc] = cleaned;
	return (1);
}

static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

char	**parse_argv_with_redirections(t_token **tokens)
{
	int		argc;
	char	**argv;
	t_token	*token;
	int		result;

	argc = 0;
	token = *tokens;
	argv = malloc(sizeof(char *) * 1);
	if (!argv)
		return (NULL);
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_WORD)
		{
			argv = extend_argv(argv, argc + 2);
			if (!argv)
				return (NULL);
			result = add_cleaned_arg(argv, token, argc);
			if (result == 0)
			{
				free_split(argv);
				return (NULL);
			}
			if (result == 1)  // Solo incrementar si realmente se añadió
				argc++;
			token = token->next;
		}
		else if (is_redirection_token(token->type))
		{
			if (token->next && token->next->type == TOKEN_WORD)
				token = token->next->next;
			else
				token = token->next;
		}
		else
			token = token->next;
	}
	argv[argc] = NULL;
	*tokens = token;
	return (argv);
}

char	**parse_argv(t_token **tokens)
{
	int		argc;
	char	**argv;
	t_token	*token;
	int		result;

	argc = 0;
	token = *tokens;
	argv = malloc(sizeof(char *) * 1);
	if (!argv)
		return (NULL);
	while (token && token->type == TOKEN_WORD)
	{
		argv = extend_argv(argv, argc + 2);
		if (!argv)
			return (NULL);
		result = add_cleaned_arg(argv, token, argc);
		if (result == 0)
		{
			free_split(argv);
			return (NULL);
		}
		if (result == 1)  // Solo incrementar si realmente se añadió
			argc++;
		token = token->next;
	}
	argv[argc] = NULL;
	*tokens = token;
	return (argv);
}
