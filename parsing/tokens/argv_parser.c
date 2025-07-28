/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/28 22:28:44 by rmakende         ###   ########.fr       */
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

	cleaned = remove_quotes(token->value);
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
