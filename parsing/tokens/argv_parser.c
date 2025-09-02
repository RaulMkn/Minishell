/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/19 15:19:11 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	add_cleaned_arg(char **argv, t_token *token, int argc)
{
	char	*cleaned;

	if (!token || !token->value || token->value[0] == '\0')
		return (-1);
	cleaned = remove_quotes(token->value);
	if (!cleaned)
		return (0);
	argv[argc] = cleaned;
	return (1);
}

static int	process_token(t_token **token_ptr, char ***argv, int *argc)
{
	int	result;

	if ((*token_ptr)->type == TOKEN_WORD)
	{
		*argv = ft_str_array_extend(*argv, *argc + 2);
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
	argv = ft_calloc(1, sizeof(char *));
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
