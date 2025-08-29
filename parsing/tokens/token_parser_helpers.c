/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/27 00:00:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	setup_empty_command(t_command *cmd)
{
	free(cmd->argv);
	cmd->argv = malloc(sizeof(char *) * 2);
	if (!cmd->argv)
	{
		c_redir(cmd->redir);
		free(cmd);
		return (0);
	}
	cmd->argv[0] = ft_strdup(":");
	if (!cmd->argv[0])
	{
		free(cmd->argv);
		c_redir(cmd->redir);
		free(cmd);
		return (0);
	}
	cmd->argv[1] = NULL;
	return (1);
}

int	parse_command_parts(t_command *cmd, t_token **curr_token)
{
	t_token	*start_token;

	start_token = *curr_token;
	cmd->argv = pars_argv_redirections(curr_token);
	if (!cmd->argv)
		return (0);
	*curr_token = start_token;
	cmd->redir = parse_redirections_mixed(curr_token);
	if (!cmd->argv[0] && cmd->redir)
		return (setup_empty_command(cmd));
	return (1);
}
