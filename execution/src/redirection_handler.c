/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/28 21:25:28 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Encuentra la última redirección de entrada
*/
static t_redir	*find_last_input_redirection(t_redir *redirs)
{
	t_redir	*last_input;
	t_redir	*current;

	last_input = NULL;
	current = redirs;
	
	while (current)
	{
		if (current->type == REDIR_IN)
			last_input = current;
		current = current->next;
	}
	
	return (last_input);
}

/*
** Encuentra la última redirección de salida (normal o append)
*/
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

/*
** Aplica una redirección específica y maneja errores
*/
static int	apply_single_redirection(t_redir *redir)
{
	int	fd;

	if (!redir)
		return (1);

	if (redir->type == REDIR_IN)
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
		{
			perror(redir->file);
			return (0);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_OUT)
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(redir->file);
			return (0);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror(redir->file);
			return (0);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	return (1);
}

/*
** Procesa redirecciones intermedias para verificar errores
*/
static int	process_intermediate_redirections(t_redir *redirs, t_redir *last_input, t_redir *last_output)
{
	t_redir	*current;
	int		fd;

	current = redirs;
	
	while (current)
	{
		if (current == last_input || current == last_output)
		{
			current = current->next;
			continue;
		}
		if (current->type == REDIR_IN)
		{
			fd = open(current->file, O_RDONLY);
			if (fd == -1)
			{
				perror(current->file);
				return (0);
			}
			close(fd);
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			int flags = O_WRONLY | O_CREAT;
			if (current->type == REDIR_OUT)
				flags |= O_TRUNC;
			else
				flags |= O_APPEND;
				
			fd = open(current->file, flags, 0644);
			if (fd == -1)
			{
				perror(current->file);
				return (0);
			}
			close(fd);
		}
		
		current = current->next;
	}
	
	return (1);
}

int	handle_redirections(t_redir *redirs)
{
	t_redir	*last_input;
	t_redir	*last_output;

	if (!redirs)
		return (1);
	
	last_input = find_last_input_redirection(redirs);
	last_output = find_last_output_redirection(redirs);
	if (!process_intermediate_redirections(redirs, last_input, last_output))
		return (0);
	if (!apply_single_redirection(last_input))
		return (0);
	if (!apply_single_redirection(last_output))
		return (0);
	
	return (1);
}
