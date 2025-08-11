/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/11 20:01:30 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_redir	*find_last_input_redirection(t_redir *redirs)
{
	t_redir	*last_input;
	t_redir	*current;

	last_input = NULL;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == HEREDOC)
			last_input = current;
		current = current->next;
	}
	return (last_input);
}

static t_redir	*find_last_output_redirection(t_redir *redirs)
{
	t_redir	*last_output;
	t_redir	*current;

	last_output = NULL;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			last_output = current;
		current = current->next;
	}
	return (last_output);
}

static int	apply_redirection(t_redir *redir)
{
	int	fd;

	if (!redir)
		return (1);
	if (redir->type == REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (1);
	if (fd == -1)
	{
		perror(redir->file);
		return (0);
	}
	if (redir->type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

static int	validate_redirections_sequentially(t_redir *redirs)
{
	t_redir	*current;
	int		fd;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
			fd = open(current->file, O_RDONLY);
		else if (current->type == REDIR_OUT)
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (current->type == REDIR_APPEND)
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = -2;
		if (fd == -1)
		{
			perror(current->file);
			return (0);
		}
		if (fd >= 0)
			close(fd);
		current = current->next;
	}
	return (1);
}

int	handle_multiple_redirections(t_redir *redirs)
{
	t_redir	*last_input;
	t_redir	*last_output;

	if (!redirs)
		return (1);
	if (!validate_redirections_sequentially(redirs))
		return (0);
	last_input = find_last_input_redirection(redirs);
	last_output = find_last_output_redirection(redirs);
	if (!apply_redirection(last_input))
		return (0);
	if (!apply_redirection(last_output))
		return (0);
	return (1);
}