/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 21:15:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/08/13 21:15:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_temp_file(void)
{
	int	fd;

	fd = open("/tmp/heredoc_temp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc: temp file creation");
		return (-1);
	}
	return (fd);
}

static int	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	g_in_heredoc = 1;
	while (1)
	{
		line = readline("> ");
		if (g_signal_received == SIGINT)
		{
			if (line)
				free(line);
			g_in_heredoc = 0;
			return (-1);
		}
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", 2);
			break;
		}
		if (ft_strlen(line) == delimiter_len &&
			ft_strncmp(line, delimiter, delimiter_len) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	g_in_heredoc = 0;
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int	temp_fd;
	int	read_fd;

	if (!delimiter)
		return (-1);
	g_signal_received = 0;
	temp_fd = create_temp_file();
	if (temp_fd == -1)
		return (-1);
	if (read_heredoc_input(temp_fd, delimiter) == -1)
	{
		close(temp_fd);
		unlink("/tmp/heredoc_temp");
		shell->last_status = 130;
		return (-1);
	}
	close(temp_fd);
	read_fd = open("/tmp/heredoc_temp", O_RDONLY);
	if (read_fd == -1)
	{
		perror("heredoc: temp file read");
		unlink("/tmp/heredoc_temp");
		return (-1);
	}
	dup2(read_fd, STDIN_FILENO);	close(read_fd);
	unlink("/tmp/heredoc_temp");
	return (0);
}




