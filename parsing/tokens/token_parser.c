/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/16 20:32:15 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_command	*init_new_command(t_token **curr_token)
{
	t_command	*new_cmd;
	t_token		*start_token;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	start_token = *curr_token;
	new_cmd->argv = pars_argv_redirections(curr_token);
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (NULL);
	}
	*curr_token = start_token;
	new_cmd->redir = parse_redirections_mixed(curr_token);
	new_cmd->next = NULL;
	return (new_cmd);
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
	if (!*cmd_list)
		*cmd_list = new_cmd;
	else
		(*current_cmd)->next = new_cmd;
	*current_cmd = new_cmd;
	return (1);
}

static int	handle_pipe_token(t_token **curr_token, t_command *current_cmd)
{
	if (!current_cmd || !(*curr_token)->next)
		return (0);
	*curr_token = (*curr_token)->next;
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
				return (NULL);
		}
		else if (curr_token->type == TOKEN_PIPE)
		{
			if (!handle_pipe_token(&curr_token, *current_cmd))
				return (clear_command(*cmd_list), NULL);
		}
		else
			curr_token = curr_token->next;
	}
	return (*cmd_list);
}

static t_token	*concatenate_consecutive_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;
	char	*new_value;
	int		has_single_quotes;
	char	*unquoted;

	current = tokens;
	while (current && current->next)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && next->type == TOKEN_WORD)
		{
			// Detectar si el token actual tiene comillas simples
			has_single_quotes = ft_strchr(current->value, '\'') != NULL;

			if ((ft_strchr(current->value, '\'') && next->value[0] == '$')
						|| (current->value[ft_strlen(current->value)
						- 1] == '\'' && next->value[0])
					|| (ft_strchr(current->value, '"') && next->value[0] == '$')
					|| (current->value[ft_strlen(current->value) - 1] == '"'
						&& next->value[0]))
			{

				if (has_single_quotes && next->value[0] == '$')
				{
					// Eliminar las comillas del primer token para obtener su contenido literal
					unquoted = remove_quotes(current->value);
					new_value = ft_strjoin(unquoted, next->value);
					free(unquoted);
				}
				else
				{
					new_value = ft_strjoin(current->value, next->value);
				}
				if (new_value)
				{
					free(current->value);
					current->value = new_value;
					current->next = next->next;
					free(next->value);
					free(next);
					continue ;
				}
			}
		}
		current = current->next;
	}
	return (tokens);
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
