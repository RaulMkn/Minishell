/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:56:53 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/07 19:17:35 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_var_value(char *var_name, char **env, int last_status)
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
		if (ft_strncmp(env[i], var_name, var_len) == 0 && 
			env[i][var_len] == '=')
			return (ft_strdup(env[i] + var_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

static char	*build_result(char *current, char *to_add)
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

char	*expand_variables(char *str, char **env, int last_status)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		i;
	int		start;
	int		len;

	if (!str || !env)
		return (NULL);
	len = ft_strlen(str);
	// Si el string está completamente rodeado de comillas simples, no expandir
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		return (ft_strdup(str));
	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '"')
		{
			start = ++i;
			if (str[i] == '?')
				i++;
			else
			{
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
			}
			var_name = ft_substr(str, start, i - start);
			var_value = get_var_value(var_name, env, last_status);
			result = build_result(result, var_value);
			free(var_name);
			free(var_value);
		}
		else
		{
			temp = ft_substr(str, i, 1);
			result = build_result(result, temp);
			free(temp);
			i++;
		}
	}
	return (result ? result : ft_strdup(""));
}

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
	if (state->error_msg)
		free(state->error_msg);
	state->error = error;
	state->error_msg = ft_strdup(msg);
}
