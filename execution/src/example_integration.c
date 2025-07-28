/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example_integration.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/28 22:29:24 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_command	*init_new_command_improved(t_token **curr_token)
{
	t_command	*new_cmd;

	new_cmd = parse_command_unified(curr_token);
	if (!new_cmd)
		return (NULL);
	if (!validate_redirections(new_cmd->redir))
	{
		clear_command_single(new_cmd);
		return (NULL);
	}
	return (new_cmd);
}

int	execute_pipeline_improved(t_command *cmd_list, char ***mini_env)
{
	if (!validate_pipeline_redirections(cmd_list))
		return (1);
	return (execute_pipeline(cmd_list, mini_env));
}

void	example_test_cases(void)
{
}
