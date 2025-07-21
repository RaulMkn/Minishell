/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:48:24 by rmakende         ###   ########.fr       */
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

static char	*clean_quotes_loop(char *str, char *clean)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				clean[j++] = str[i++];
			if (str[i])
				i++;
		}
		else
			clean[j++] = str[i++];
	}
	clean[j] = '\0';
	return (clean);
}

static char	*clean_quotes(char *str)
{
	char	*clean;

	if (!str)
		return (NULL);
	clean = malloc(strlen(str) + 1);
	if (!clean)
		return (NULL);
	return (clean_quotes_loop(str, clean));
}

static int	add_cleaned_arg(char **argv, t_token *token, int argc)
{
	char	*cleaned;

	cleaned = clean_quotes(token->value);
	if (!cleaned)
		return (0);
	argv[argc] = cleaned;
	return (1);
}

char	**parse_argv(t_token **tokens)
{
	int		argc;
	char	**argv;
	t_token	*token;

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
		if (!add_cleaned_arg(argv, token, argc))
		{
			free_split(argv);
			return (NULL);
		}
		argc++;
		token = token->next;
	}
	argv[argc] = NULL;
	*tokens = token;
	return (argv);
}
