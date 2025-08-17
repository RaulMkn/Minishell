/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:56:37 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/08/17 15:57:27 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static volatile sig_atomic_t	interrupted = 0;

static void	sigint_handler(int sig)
{
	(void)sig;
	interrupted = 1;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static int	update_quote_state(char c, int state)
{
	if (c == '\'' && state != QUOTE_DOUBLE)
	{
		if (state == QUOTE_SINGLE)
			return (QUOTE_NONE);
		else
			return (QUOTE_SINGLE);
	}
	else if (c == '\"' && state != QUOTE_SINGLE)
	{
		if (state == QUOTE_DOUBLE)
			return (QUOTE_NONE);
		else
			return (QUOTE_DOUBLE);
	}
	return (state);
}

static int	check_quote_state_at_position(char *input, size_t i)
{
	int	j;
	int	state;

	if (!input || i == 0)
		return (QUOTE_NONE);
	state = QUOTE_NONE;
	j = 0;
	while (j < (int)i)
	{
		state = update_quote_state(input[j], state);
		j++;
	}
	return (state);
}

static int	check_forbidden_operators(char *input, size_t i, t_shell *shell)
{
	if (input[i] == '<' && input[i + 1] == '>')
	{
		set_error(&shell->lexer_state, ERROR_SYNTAX,
			"syntax error near unexpected token `newline'");
		return (1);
	}
	if (input[i] == '&' && input[i + 1] == '&')
		return (1);
	if (input[i] == '|' && input[i + 1] == '|')
		return (1);
	return (0);
}

int	is_forbidden_sequence(char *input, size_t i, t_shell *shell)
{
	int	state;

	(void)shell;
	state = check_quote_state_at_position(input, i);
	if (state != QUOTE_NONE)
		return (0);
	return (check_forbidden_operators(input, i, shell));
}

int	handle_unclosed_quotes(t_shell *shell, char **buffer)
{
	void	(*old_handler)(int);
	char	*extra;
	char	*tmp;

	old_handler = signal(SIGINT, sigint_handler);
	interrupted = 0;
	while (shell->lexer_state.quote_state != QUOTE_NONE)
	{
		extra = readline("> ");
		if (!extra || interrupted)
		{
			free(*buffer);
			*buffer = NULL;
			signal(SIGINT, old_handler);
			interrupted = 0;
			if (extra)
				free(extra);
			return (0);
		}
		tmp = ft_strjoin(*buffer, extra);
		free(*buffer);
		*buffer = tmp;
		free(extra);
		shell->lexer_state.quote_state = check_quote_state_at_position(*buffer,
				ft_strlen(*buffer));
	}
	signal(SIGINT, old_handler);
	return (1);
}
