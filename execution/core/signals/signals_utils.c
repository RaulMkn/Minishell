/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:27:57 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/08/15 18:28:07 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_signal_state	*get_signal_state(void)
{
	static t_signal_state	state = {0, 0};

	return (&state);
}

void	set_signal_received(int signal)
{
	get_signal_state()->signal_received = signal;
}

void	set_heredoc_state(int state)
{
	get_signal_state()->in_heredoc = state;
}

int	get_signal_received(void)
{
	return (get_signal_state()->signal_received);
}
