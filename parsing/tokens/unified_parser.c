/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unified_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:30:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/07 18:56:40 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static t_redir	*create_redirection_from_tokens(t_token *redir_token,
		t_token *file_token)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	if (redir_token->type == TOKEN_REDIR_IN)
		new_redir->type = REDIR_IN;
	else if (redir_token->type == TOKEN_REDIR_OUT)
		new_redir->type = REDIR_OUT;
	else if (redir_token->type == TOKEN_APPEND)
		new_redir->type = REDIR_APPEND;
	else if (redir_token->type == TOKEN_HEREDOC)
		new_redir->type = HEREDOC;
	new_redir->file = remove_quotes(file_token->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->next = NULL;
	return (new_redir);
}

static void	add_redir_to_list(t_redir **redir_list, t_redir *new_redir)
{
	t_redir	*current;

	if (!*redir_list)
		*redir_list = new_redir;
	else
	{
		current = *redir_list;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

static char	**extend_argv_array(char **argv, int new_size)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * new_size);
	if (!new_argv)
		return (NULL);
	i = 0;
	if (argv)
	{
		while (argv[i] && i < new_size - 1)
		{
			new_argv[i] = argv[i];
			i++;
		}
		free(argv);
	}
	while (i < new_size)
	{
		new_argv[i] = NULL;
		i++;
	}
	return (new_argv);
}

t_command	*parse_command_unified(t_token **tokens)
{
	t_command	*cmd;
	t_token		*current;
	char		**argv;
	int			argc;
	t_redir		*new_redir;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	argv = malloc(sizeof(char *));
	if (!argv)
	{
		free(cmd);
		return (NULL);
	}
	argv[0] = NULL;
	argc = 0;
	current = *tokens;
	cmd->redir = NULL;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			argv = extend_argv_array(argv, argc + 2);
			if (!argv)
			{
				clear_redir_list(cmd->redir);
				free(cmd);
				return (NULL);
			}
			argv[argc] = remove_quotes(current->value);
			if (!argv[argc])
			{
				free_split(argv);
				clear_redir_list(cmd->redir);
				free(cmd);
				return (NULL);
			}
			argc++;
			current = current->next;
		}
		else if (is_redirection_token(current->type))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				free_split(argv);
				clear_redir_list(cmd->redir);
				free(cmd);
				return (NULL);
			}
			new_redir = create_redirection_from_tokens(current, current->next);
			if (!new_redir)
			{
				free_split(argv);
				clear_redir_list(cmd->redir);
				free(cmd);
				return (NULL);
			}
			add_redir_to_list(&cmd->redir, new_redir);
			current = current->next->next;
		}
		else
		{
			current = current->next;
		}
	}
	cmd->argv = argv;
	cmd->next = NULL;
	*tokens = current;
	return (cmd);
}
