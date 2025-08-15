/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unified_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 22:47:23 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/15 18:10:56 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_redir	*create_redirection_tn(t_token *redir_token,
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

void	add_redir_to_list(t_redir **redir_list, t_redir *new_redir)
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

char	**extend_argv_array(char **argv, int new_size)
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

int	process_word_token(t_token **current, char ***argv, int *argc)
{
	*argv = extend_argv_array(*argv, *argc + 2);
	if (!*argv)
		return (0);
	(*argv)[*argc] = remove_quotes((*current)->value);
	if (!(*argv)[*argc])
		return (0);
	(*argc)++;
	*current = (*current)->next;
	return (1);
}

int	process_redirection_token(t_token **current, t_redir **redir_list)
{
	t_redir	*new_redir;

	if (!(*current)->next || (*current)->next->type != TOKEN_WORD)
		return (0);
	new_redir = create_redirection_tn(*current, (*current)->next);
	if (!new_redir)
		return (0);
	add_redir_to_list(redir_list, new_redir);
	*current = (*current)->next->next;
	return (1);
}
