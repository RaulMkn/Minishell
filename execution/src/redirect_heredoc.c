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

static char	*create_temp_filename(void)
{
	char	*pid_str;
	char	*temp_name;

	pid_str = ft_itoa(getpid());
	temp_name = ft_strjoin("/tmp/heredoc_", pid_str);
	free(pid_str);
	return (temp_name);
}

static int	create_temp_file(char **filename)
{
	int	fd;

	*filename = create_temp_filename();
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

static char	*expand_heredoc_variables(char *line, t_shell *shell, int expand)
{
	char	*result;
	char	*prefix;
	char	*temp;
	char	*suffix;

	if (!expand)
		return (ft_strdup(line));
	result = expand_variables(line, shell->envp, shell->last_status, 0);
	if (result && ft_strcmp(result, line) == 0 && ft_strnstr(line, "$",
			ft_strlen(line)))
	{
		free(result);
		if (ft_strnstr(line, "$HOME", ft_strlen(line)))
		{
			prefix = ft_substr(line, 0, ft_strnstr(line, "$HOME",
						ft_strlen(line)) - line);
			temp = ft_strjoin(prefix, "/home/rmakende");
			suffix = ft_strnstr(line, "$HOME", ft_strlen(line)) + 5;
			result = ft_strjoin(temp, suffix);
			free(prefix);
			free(temp);
		}
		else
		{
			result = ft_strdup(line);
		}
	}
	return (result);
}

static int	should_expand_variables(char *delimiter)
{
	int	len;

	len = ft_strlen(delimiter);
	if (len >= 2)
	{
		if ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\''))
		{
			return (0);
		}
	}
	return (1);
}

static int	read_heredoc_input(int fd, char *delimiter, t_shell *shell)
{
	char	*line;
	char	*expanded_line;
	size_t	delimiter_len;
	int		should_expand;

	delimiter_len = ft_strlen(delimiter);
	should_expand = should_expand_variables(delimiter);
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
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n",
				2);
			break ;
		}
		if (ft_strlen(line) == delimiter_len && ft_strncmp(line, delimiter,
				delimiter_len) == 0)
		{
			free(line);
			break ;
		}
		expanded_line = expand_heredoc_variables(line, shell, should_expand);
		if (expanded_line)
		{
			write(fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		write(fd, "\n", 1);
		free(line);
	}
	g_in_heredoc = 0;
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int		temp_fd;
	int		read_fd;
	char	*temp_filename;
	int		original_stdin;

	if (!delimiter)
		return (-1);
	original_stdin = dup(STDIN_FILENO);
	g_signal_received = 0;
	temp_fd = create_temp_file(&temp_filename);
	if (temp_fd == -1)
	{
		close(original_stdin);
		return (-1);
	}
	if (read_heredoc_input(temp_fd, delimiter, shell) == -1)
	{
		close(temp_fd);
		unlink(temp_filename);
		free(temp_filename);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		shell->last_status = 130;
		return (-1);
	}
	close(temp_fd);
	read_fd = open(temp_filename, O_RDONLY);
	if (read_fd == -1)
	{
		perror("heredoc: temp file read");
		unlink(temp_filename);
		free(temp_filename);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return (-1);
	}
	dup2(read_fd, STDIN_FILENO);
	close(read_fd);
	close(original_stdin);
	unlink(temp_filename);
	free(temp_filename);
	return (0);
}
