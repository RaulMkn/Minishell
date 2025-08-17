/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:30:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/08/17 15:47:04 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_command(t_command *cmd)
{
	int		i;
	t_redir	*tmp;

	if (!cmd)
		return ;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
	}
	while (cmd->redir)
	{
		tmp = cmd->redir->next;
		free(cmd->redir->file);
		free(cmd->redir);
		cmd->redir = tmp;
	}
	free(cmd);
}

void	set_error(t_lexer_state *state, t_error_type error, char *msg)
{
	char	*new_msg;

	new_msg = ft_strdup(msg);
	if (!new_msg)
		return ;
	if (state->error_msg)
		free(state->error_msg);
	state->error = error;
	state->error_msg = new_msg;
}

char	*get_var_value(char *var_name, char **env, int last_status)
{
	int	i;
	int	var_len;

	if (!var_name)
		return (NULL);
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(last_status));
	var_len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, var_len) == 0
			&& env[i][var_len] == '=')
			return (ft_strdup(env[i] + var_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*build_result(char *current, char *to_add)
{
	char	*result;

	if (!current)
		return (ft_strdup(to_add));
	if (!to_add)
		return (current);
	result = ft_strjoin(current, to_add);
	free(current);
	return (result);
}

int	get_var_name_length(char *str, int start)
{
	int	i;

	i = start;
	if (str[i] == '?')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i - start);
}
