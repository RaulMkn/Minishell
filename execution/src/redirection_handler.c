/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:41:02 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_output_redirection(char *file, int append)
{
	int	fd;

	if (append)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd != -1)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

static void	handle_input_redirection(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	handle_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_OUT)
			handle_output_redirection(current->file, 0);
		else if (current->type == REDIR_IN)
			handle_input_redirection(current->file);
		else if (current->type == REDIR_APPEND)
			handle_output_redirection(current->file, 1);
		current = current->next;
	}
}
