/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/15 18:10:10 by ruortiz-         ###   ########.fr       */
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
		return (-1);
	cleaned = remove_quotes(token->value);
	if (!cleaned)
		return (0);
	if (cleaned[0] == '\0')
	{
		free(cleaned);
		return (-1);
	}
	argv[argc] = cleaned;
	return (1);
}

static int	process_token(t_token **token_ptr, char ***argv, int *argc)
{
	int	result;

	if ((*token_ptr)->type == TOKEN_WORD)
	{
		*argv = extend_argv(*argv, *argc + 2);
		if (!*argv)
			return (0);
		result = add_cleaned_arg(*argv, *token_ptr, *argc);
		if (result == 0)
			return (-1);
		if (result == 1)
			(*argc)++;
		*token_ptr = (*token_ptr)->next;
	}
	else if (is_redirection_token((*token_ptr)->type))
	{
		if ((*token_ptr)->next && (*token_ptr)->next->type == TOKEN_WORD)
			*token_ptr = (*token_ptr)->next->next;
		else
			*token_ptr = (*token_ptr)->next;
	}
	else
		*token_ptr = (*token_ptr)->next;
	return (1);
}

char	**pars_argv_redirections(t_token **tokens)
{
	int		argc;
	char	**argv;
	t_token	*token;
	int		status;

	argc = 0;
	token = *tokens;
	argv = malloc(sizeof(char *));
	if (!argv)
		return (NULL);
	while (token && token->type != TOKEN_PIPE)
	{
		status = process_token(&token, &argv, &argc);
		if (status == 0 || status == -1)
		{
			free_split(argv);
			return (NULL);
		}
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
			return (free_split(argv), NULL);
		if (result == 1)
			argc++;
		token = token->next;
	}
	argv[argc] = NULL;
	*tokens = token;
	return (argv);
}
