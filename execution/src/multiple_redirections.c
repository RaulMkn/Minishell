/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/06 20:19:31 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Encuentra la última redirección de entrada
** En bash: cat <file1 <file2 <file3 → solo file3 se usa como entrada
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
static int	validate_and_create_all_redirections(t_redir *redirs,
		t_redir *last_output)
{
	t_redir	*current;
	int		fd;
	int		has_error;
	char	*error_file;

	current = redirs;
	has_error = 0;
	error_file = NULL;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			if (access(current->file, R_OK) != 0)
			{
				if (!has_error)
				{
					error_file = current->file;
					has_error = 1;
				}
			}
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (current != last_output)
			{
				fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
				{
					if (!has_error)
					{
						error_file = current->file;
						has_error = 1;
					}
				}
				else
				{
					close(fd);
				}
			}
		}
		current = current->next;
	}
	if (has_error)
	{
		perror(error_file);
		return (0);
	}
	return (1);
}

int	handle_multiple_redirections(t_redir *redirs)
{
	t_redir *last_input;
	t_redir *last_output;

	if (!redirs)
		return (1);

	// Encontrar las redirecciones efectivas
	last_input = find_last_input_redirection(redirs);
	last_output = find_last_output_redirection(redirs);

	// 🔧 CREAR todos los archivos y validar (sin last_input)
	if (!validate_and_create_all_redirections(redirs, last_output))
		return (0);

	// Aplicar solo las redirecciones efectivas
	if (last_input && !apply_single_redirection(last_input))
		return (0);

	if (last_output && !apply_single_redirection(last_output))
		return (0);

	return (1);
}