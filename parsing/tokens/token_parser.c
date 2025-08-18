/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/18 17:30:25 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_command	*init_new_command(t_token **curr_token)
{
	t_command	*cmd;
	t_token		*start_token;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	start_token = *curr_token;
	cmd->argv = pars_argv_redirections(curr_token);
	if (!cmd->argv)
		return (free(cmd), NULL);
	*curr_token = start_token;
	cmd->redir = parse_redirections_mixed(curr_token);
	if (!cmd->argv[0] && cmd->redir)
	{
		free(cmd->argv);
		cmd->argv = malloc(sizeof(char *) * 2);
		if (!cmd->argv)
			return (c_redir(cmd->redir), free(cmd), (NULL));
		cmd->argv[0] = ft_strdup(":");
		if (!cmd->argv[0])
			return (free(cmd->argv), c_redir(cmd->redir), free(cmd), NULL);
		cmd->argv[1] = NULL;
	}
	return (cmd->next = NULL, cmd);
}

static int	handle_word_token(t_token **curr_token, t_command **cmd_list,
		t_command **current_cmd)
{
	t_command	*new_cmd;

	new_cmd = init_new_command(curr_token);
	if (!new_cmd)
		return (0);
	if (!*cmd_list)
		*cmd_list = new_cmd;
	else
		(*current_cmd)->next = new_cmd;
	*current_cmd = new_cmd;
	return (1);
}

static t_command	*parse_tokens_loop(t_token *curr_token,
		t_command **cmd_list, t_command **current_cmd)
{
	while (curr_token)
	{
		if (curr_token->type == TOKEN_WORD
			|| is_redirection_token(curr_token->type))
		{
			if (!handle_word_token(&curr_token, cmd_list, current_cmd))
			{
				clear_command(*cmd_list);
				return (NULL);
			}
		}
		else if (curr_token->type == TOKEN_PIPE)
		{
			if (!handle_pipe_token(&curr_token, *current_cmd))
			{
				clear_command(*cmd_list);
				return (NULL);
			}
		}
		else
			curr_token = curr_token->next;
	}
	return (*cmd_list);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*curr_token;

	cmd_list = NULL;
	current_cmd = NULL;
	curr_token = tokens;
	curr_token = concatenate_consecutive_tokens(curr_token);
	return (parse_tokens_loop(curr_token, &cmd_list, &current_cmd));
}
