/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:40:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/28 21:25:28 by rmakende         ###   ########.fr       */
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

/*
** Encuentra la última redirección de salida (normal o append)
** En bash: echo hi >file1 >file2 >>file3 → solo file3 se usa (append mode)
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
** En bash: ls >file1 >invalid_permission >file3
** - Se intenta abrir file1 (éxito)
** - Se intenta abrir invalid_permission (falla → error)
** - No se llega a procesar file3
*/
static int	process_intermediate_redirections(t_redir *redirs, t_redir *last_input, t_redir *last_output)
{
	t_redir	*current;
	int		fd;

	current = redirs;
	
	while (current)
	{
		// Saltar las redirecciones que serán aplicadas finalmente
		if (current == last_input || current == last_output)
		{
			current = current->next;
			continue;
		}
		
		// Verificar redirecciones intermedias para detectar errores
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

/*
** Maneja redirecciones múltiples siguiendo el comportamiento de bash
** 1. Procesa todas las redirecciones para detectar errores
** 2. Solo aplica las últimas de cada tipo (entrada y salida)
*/
int	handle_multiple_redirections(t_redir *redirs)
{
	t_redir	*last_input;
	t_redir	*last_output;

	if (!redirs)
		return (1);
	
	// Encontrar las redirecciones efectivas
	last_input = find_last_input_redirection(redirs);
	last_output = find_last_output_redirection(redirs);
	
	// Procesar redirecciones intermedias para verificar errores
	if (!process_intermediate_redirections(redirs, last_input, last_output))
		return (0);
	
	// Aplicar solo las redirecciones efectivas
	if (!apply_single_redirection(last_input))
		return (0);
	
	if (!apply_single_redirection(last_output))
		return (0);
	
	return (1);
}
