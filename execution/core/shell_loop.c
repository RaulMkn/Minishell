/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:45:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 21:54:50 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_redirections_only(t_command *new, t_shell *shell)
{
	t_redir	*r;
	int		flags;
	int		fd;

	r = new->redir;
	while (r)
	{
		if (r->type == REDIR_OUT || r->type == REDIR_APPEND)
		{
			flags = O_WRONLY | O_CREAT;
			if (r->type == REDIR_APPEND)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(r->file, flags, 0644);
			if (fd != -1)
				close(fd);
		}
		r = r->next;
	}
	shell->last_status = 0;
	clear_command(new);
}

static void	process_valid_tokens(t_shell *shell, t_token *tokens)
{
	t_command	*new;

	if (!tokens)
		return (shell->last_status = 0, (void)0);
	if (!is_valid_operator_sequence(tokens))
		return (ft_putendl_fd("Error de sintaxis", 2), (void)0);
	new = parse_tokens(tokens);
	if (!new)
		return (shell->last_status = 0, (void)0);
	if ((!new->argv || !new->argv[0]) && new->redir)
		return (handle_redirections_only(new, shell), (void)0);
	if (!new->argv || !new->argv[0] || new->argv[0][0] == '\0')
		return (shell->last_status = 0, clear_command(new), (void)0);
	if (shell->cmd_list)
		clear_command(shell->cmd_list);
	shell->cmd_list = new;
	shell->last_status = execute_pipeline(shell->cmd_list, &shell->envp, shell);
}

static void	process_input_line(t_shell *shell, char *line)
{
	t_token	*tokens;

	add_history(line);
	tokens = tokenize_input(line, shell);
	if (shell->lexer_state.error == ERROR_NONE)
	{
		if (tokens)
			expand_and_filter_tokens(&tokens, shell);
		process_valid_tokens(shell, tokens);
	}
	else if (shell->lexer_state.error_msg)
	{
		ft_putendl_fd(shell->lexer_state.error_msg, 2);
		free(shell->lexer_state.error_msg);
		shell->lexer_state.error_msg = NULL;
		shell->lexer_state.error = ERROR_NONE;
	}
	if (tokens)
		clear_tokens(&tokens);
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		if (get_signal_received() == SIGINT)
		{
			shell->last_status = 130;
			set_signal_received(0);
		}
		line = readline("minishell$ ");
		if (!line)
		{
			if (get_signal_received() == SIGINT)
			{
				shell->last_status = 130;
				set_signal_received(0);
				continue ;
			}
			ft_printf("exit\n");
			break ;
		}
		if (*line)
			process_input_line(shell, line);
		free(line);
	}
}
