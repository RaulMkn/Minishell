/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/14 23:29:06 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input_redirection(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redirection(char *filename, int append)
{
	int	fd;
	int	flags;

	if (!filename)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_multiple_redirections(t_redir *redirs, t_shell *shell)
{
	t_redir *current;

	(void)shell;
	if (!redirs)
		return (1);

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			if (handle_input_redirection(current->file) == -1)
				return (0);
		}
		else if (current->type == REDIR_OUT)
		{
			if (handle_output_redirection(current->file, 0) == -1)
				return (0);
		}
		else if (current->type == REDIR_APPEND)
		{
			if (handle_output_redirection(current->file, 1) == -1)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
