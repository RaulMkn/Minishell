/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_validator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/11 20:27:10 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Valida si un archivo de entrada existe y es legible
*/
static int	validate_input_file(const char *filename)
{
	int	fd;

	if (!filename)
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	close(fd);
	return (1);
}

/*
** Valida si un archivo de salida puede ser creado/escrito
*/
static int	validate_output_file(const char *filename)
{
	int		fd;
	char	*dir_path;
	char	*last_slash;

	if (!filename)
		return (0);
	fd = open(filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		dir_path = ft_strdup(filename);
		if (!dir_path)
			return (0);
		last_slash = ft_strrchr(dir_path, '/');
		if (last_slash)
		{
			*last_slash = '\0';
			if (access(dir_path, W_OK) == -1)
			{
				return (ft_putstr_fd("minishell: ", 2), perror(dir_path),
					free(dir_path), 0);
			}
		}
		return (free(dir_path), perror(filename), (0));
	}
	return (close(fd), 1);
}

/*
** Cuenta redirecciones del mismo tipo para determinar cuál usar
** En bash, solo la última redirección de cada tipo es efectiva
*/
static t_redir	*get_effective_redirections(t_redir *redirs)
{
	t_redir	*effective_list;
	t_redir	*current;
	t_redir	*last_in;
	t_redir	*last_out;
	t_redir	*last_append;
	t_redir	*new_redir;
	t_redir	*new_redir;
	t_redir	*new_redir;

	effective_list = NULL;
	last_in = NULL;
	last_out = NULL;
	last_append = NULL;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
			last_in = current;
		else if (current->type == REDIR_OUT)
			last_out = current;
		else if (current->type == REDIR_APPEND)
			last_append = current;
		current = current->next;
	}
	if (last_in)
	{
		new_redir = malloc(sizeof(t_redir));
		if (new_redir)
		{
			*new_redir = *last_in;
			new_redir->next = effective_list;
			effective_list = new_redir;
		}
	}
	if (last_append)
	{
		new_redir = malloc(sizeof(t_redir));
		if (new_redir)
		{
			*new_redir = *last_append;
			new_redir->next = effective_list;
			effective_list = new_redir;
		}
	}
	else if (last_out)
	{
		new_redir = malloc(sizeof(t_redir));
		if (new_redir)
		{
			*new_redir = *last_out;
			new_redir->next = effective_list;
			effective_list = new_redir;
		}
	}
	return (effective_list);
}

/*
** Valida todas las redirecciones antes de la ejecución
** Retorna 1 si todas son válidas, 0 si alguna falla
*/
int	validate_redirections(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*effective_redirs;
	int		valid;
	t_redir	*temp;

	if (!redirs)
		return (1);
	effective_redirs = get_effective_redirections(redirs);
	if (!effective_redirs)
		return (1);
	valid = 1;
	current = effective_redirs;
	while (current && valid)
	{
		if (current->type == REDIR_IN)
		{
			if (!validate_input_file(current->file))
				valid = 0;
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (!validate_output_file(current->file))
				valid = 0;
		}
		current = current->next;
	}
	while (effective_redirs)
	{
		temp = effective_redirs;
		effective_redirs = effective_redirs->next;
		free(temp);
	}
	return (valid);
}

/*
** Valida todas las redirecciones de un pipeline antes de ejecutar
*/
int	validate_pipeline_redirections(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (!validate_redirections(current->redir))
			return (0);
		current = current->next;
	}
	return (1);
}
