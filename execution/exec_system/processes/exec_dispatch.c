/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_dispatch.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:50:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:51:18 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	exec_dispatch(t_cmd *cmd, char ***mini_env)
{
	if (!cmd)
		return (1);
	if (cmd->next == NULL)
		return (exec_single(cmd, mini_env));
	else
		return (exec_pipeline(cmd, mini_env));
}
