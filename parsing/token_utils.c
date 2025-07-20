/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:56:25 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/07/13 20:10:49 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*init_new_command(t_token **curr_token)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = parse_argv(curr_token);
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (NULL);
	}
	new_cmd->redir = parse_redirections(curr_token);
	new_cmd->next = NULL;
	return (new_cmd);
}

static void	add_command_to_list(t_command **cmd_list, t_command **current, 
							t_command *new_cmd)
{
	if (!*cmd_list)
		*cmd_list = new_cmd;
	else
		(*current)->next = new_cmd;
	*current = new_cmd;
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*curr_token;
	t_command	*new_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	curr_token = tokens;
	while (curr_token)
	{
		if (curr_token->type == TOKEN_WORD)
		{
			new_cmd = init_new_command(&curr_token);
			if (!new_cmd)
				return (NULL);
			add_command_to_list(&cmd_list, &current_cmd, new_cmd);
		}
		curr_token = curr_token->next;
	}
	return (cmd_list);
}

static char	**extend_argv(char **argv, int size)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (size + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < size - 1)
	{
		new_argv[i] = argv[i];
		i++;
	}
	free(argv);
	return (new_argv);
}

char	**parse_argv(t_token **tokens)
{
	int		argc;
	char	**argv;
	t_token	*token;

	argc = 0;
	token = *tokens;
	argv = malloc(sizeof(char *) * 1);
	if (!argv)
		return (NULL);
	while (token && token->type == TOKEN_WORD)
	{
		argv = extend_argv(argv, argc + 2);
		if (!argv)
			return (NULL);
		argv[argc] = ft_strdup(token->value);
		if (!argv[argc])
			return (NULL);
		argc++;
		token = token->next;
	}
	argv[argc] = NULL;
	*tokens = token;
	return (argv);
}

static t_redir	*create_redirection(t_token *token)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->type = token->type;
	new_redir->file = ft_strdup(token->next->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->next = NULL;
	return (new_redir);
}

static t_redir	*add_new_redirection(t_redir *redir, t_token *token)
{
	t_redir	*new_redir;

	new_redir = create_redirection(token);
	if (!new_redir)
		return (NULL);
	new_redir->next = redir;
	return (new_redir);
}

t_redir	*parse_redirections(t_token **tokens)
{
	t_redir	*redir;
	t_token	*token;

	redir = NULL;
	token = *tokens;
	while (token && (token->type == TOKEN_REDIR_IN || 
			token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND || 
			token->type == TOKEN_HEREDOC))
	{
		if (!token->next || token->next->type != TOKEN_WORD)
			return (NULL);
		redir = add_new_redirection(redir, token);
		if (!redir)
			return (NULL);
		token = token->next->next;
	}
	*tokens = token;
	return (redir);
}


