/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unified_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:30:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/13 02:11:50 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	parse_tokens_to_command(t_command *cmd, t_token **tokens,
		char ***argv, int *argc)
{
	t_token	*current;

	current = *tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			if (!process_word_token(&current, argv, argc))
				return (0);
		}
		else if (is_redirection_token(current->type))
		{
			if (!process_redirection_token(&current, &cmd->redir))
				return (0);
		}
		else
			current = current->next;
	}
	*tokens = current;
	return (1);
}

t_command	*parse_command_unified(t_token **tokens)
{
	t_command	*cmd;
	char		**argv;
	int			argc;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	argv = malloc(sizeof(char *));
	if (!argv)
		return (free(cmd), NULL);
	argv[0] = NULL;
	argc = 0;
	cmd->redir = NULL;
	if (!parse_tokens_to_command(cmd, tokens, &argv, &argc))
	{
		free_split(argv);
		clear_redir_list(cmd->redir);
		free(cmd);
		return (NULL);
	}
	cmd->argv = argv;
	cmd->next = NULL;
	return (cmd);
}
