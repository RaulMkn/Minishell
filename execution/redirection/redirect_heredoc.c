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

#include "../../minishell.h"

static int	create_temp_file(char **filename, t_shell *shell)
{
	char		*counter_str;
	int			fd;

	shell->heredoc_counter++;
	counter_str = ft_itoa(shell->heredoc_counter);
	*filename = ft_strjoin("/tmp/heredoc_", counter_str);
	free(counter_str);
	fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc: temp file creation");
		free(*filename);
		*filename = NULL;
		return (-1);
	}
	return (fd);
}
static int	read_heredoc_input(int fd, char *delimiter, t_shell *shell)
{
	char	*line;
	int		eof_status;
	int		process_result;

	set_heredoc_state(1);
	while (1)
	{
		if (check_signal_interrupt(NULL, shell) == -1)
			return (-1);
		line = read_heredoc_line();
		if (check_signal_interrupt(line, shell) == -1)
			return (-1);
		eof_status = handle_eof_or_signal(line, shell);
		if (eof_status == -1)
			return (-1);
		if (eof_status == 1)
			break ;
		process_result = process_heredoc_input_line(fd, line, delimiter, shell);
		if (process_result == 0)
			return (0);
	}
	set_heredoc_state(0);
	return (0);
}

static int	open_temp_for_reading(char *filename, int original_stdin)
{
	int	read_fd;

	read_fd = open(filename, O_RDONLY);
	if (read_fd == -1)
	{
		perror("heredoc: temp file read");
		unlink(filename);
		free(filename);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return (-1);
	}
	dup2(read_fd, STDIN_FILENO);
	close(read_fd);
	close(original_stdin);
	unlink(filename);
	free(filename);
	return (0);
}

static int	write_heredoc_to_temp(char *delimiter, t_shell *shell,
		int original_stdin, char **filename)
{
	int	temp_fd;

	temp_fd = create_temp_file(filename, shell);
	if (temp_fd == -1)
	{
		close(original_stdin);
		return (-1);
	}
	if (read_heredoc_input(temp_fd, delimiter, shell) == -1)
	{
		close(temp_fd);
		unlink(*filename);
		free(*filename);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return (-1);
	}
	close(temp_fd);
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int		original_stdin;
	char	*temp_filename;

	if (!delimiter)
		return (-1);
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
	{
		perror("dup");
		return (-1);
	}
	set_signal_received(0);
	temp_filename = NULL;
	if (write_heredoc_to_temp(delimiter, shell, original_stdin,
			&temp_filename) == -1)
	{
		close(original_stdin);
		set_signal_received(0);
		return (-1);
	}
	if (open_temp_for_reading(temp_filename, original_stdin) == -1)
		return (-1);
	return (0);
}
