/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_validator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:35:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/13 21:01:04 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static t_redir	*get_effective_redirections(t_redir *redirs)
{
	t_redir	*effective_list;
	t_redir	*last_in;
	t_redir	*last_out;
	t_redir	*last_append;

	find_last_redirections(redirs, &last_in, &last_out, &last_append);
	effective_list = NULL;
	effective_list = create_effective_redir(last_in, effective_list);
	if (last_append)
		effective_list = create_effective_redir(last_append, effective_list);
	else
		effective_list = create_effective_redir(last_out, effective_list);
	return (effective_list);
}

static void	free_effective_redirections(t_redir *effective_redirs)
{
	t_redir	*temp;

	while (effective_redirs)
	{
		temp = effective_redirs;
		effective_redirs = effective_redirs->next;
		free(temp);
	}
}

int	validate_redirections(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*effective_redirs;
	int		valid;

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
	return (free_effective_redirections(effective_redirs), valid);
}

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
