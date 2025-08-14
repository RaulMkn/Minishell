/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_valdiator_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:45:42 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/13 21:01:04 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_input_file(const char *filename)
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

int	validate_directory_write(char *dir_path, char *filename)
{
	char	*last_slash;

	last_slash = ft_strrchr(dir_path, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(dir_path, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(dir_path);
			free(dir_path);
			return (0);
		}
	}
	free(dir_path);
	perror(filename);
	return (0);
}

int	validate_output_file(const char *filename)
{
	int		fd;
	char	*dir_path;

	if (!filename)
		return (0);
	fd = open(filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		dir_path = ft_strdup(filename);
		if (!dir_path)
			return (0);
		return (validate_directory_write(dir_path, (char *)filename));
	}
	close(fd);
	return (1);
}

t_redir	*create_effective_redir(t_redir *source, t_redir *list)
{
	t_redir	*new_redir;

	if (!source)
		return (list);
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (list);
	*new_redir = *source;
	new_redir->next = list;
	return (new_redir);
}

void	find_last_redirections(t_redir *redirs, t_redir **last_in,
		t_redir **last_out, t_redir **last_append)
{
	t_redir	*current;

	*last_in = NULL;
	*last_out = NULL;
	*last_append = NULL;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
			*last_in = current;
		else if (current->type == REDIR_OUT)
			*last_out = current;
		else if (current->type == REDIR_APPEND)
			*last_append = current;
		current = current->next;
	}
}
