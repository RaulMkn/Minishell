/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:35:24 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_command	*init_new_command(t_token **curr_token)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = parse_argv(curr_token);
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (NULL);
	}
	new_cmd->redir = parse_redirections(curr_token);
	new_cmd->next = NULL;
	return (new_cmd);
}

static void	add_command_to_list(t_command **cmd_list, t_command **current,
		t_command *new_cmd)
{
	if (!*cmd_list)
		*cmd_list = new_cmd;
	else
		(*current)->next = new_cmd;
	*current = new_cmd;
}

static int	handle_word_token(t_token **curr_token, t_command **cmd_list,
		t_command **current_cmd)
{
	t_command	*new_cmd;

	new_cmd = init_new_command(curr_token);
	if (!new_cmd)
	{
		clear_command(*cmd_list);
		return (0);
	}
	add_command_to_list(cmd_list, current_cmd, new_cmd);
	return (1);
}

static int	handle_pipe_token(t_token **curr_token, t_command *current_cmd)
{
	if (!current_cmd || !(*curr_token)->next)
		return (0);
	*curr_token = (*curr_token)->next;
	return (1);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*curr_token;

	cmd_list = NULL;
	current_cmd = NULL;
	curr_token = tokens;
	while (curr_token)
	{
		if (curr_token->type == TOKEN_WORD)
		{
			if (!handle_word_token(&curr_token, &cmd_list, &current_cmd))
				return (NULL);
		}
		else if (curr_token->type == TOKEN_PIPE)
		{
			if (!handle_pipe_token(&curr_token, current_cmd))
				return (clear_command(cmd_list), NULL);
		}
		else
			curr_token = curr_token->next;
	}
	return (cmd_list);
}
