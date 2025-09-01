/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_shlvl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:00:00 by ruortiz-         #+#    #+#             */
/*   Updated: 2025/09/01 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	parse_shlvl_value(char *shlvl_str)
{
	int		shlvl;
	int		flag;

	if (shlvl_str[0] == '\0')
		return (1);
	shlvl = ft_atoi(shlvl_str, &flag);
	if (flag == 0 && shlvl >= 0 && shlvl < 999)
		shlvl++;
	else
		shlvl = 1;
	return (shlvl);
}

static int	get_current_shlvl(char **envp)
{
	char	*shlvl_str;
	int		i;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			shlvl_str = envp[i] + 6;
			return (parse_shlvl_value(shlvl_str));
		}
		i++;
	}
	return (1);
}

void	init_shell_shlvl(t_shell *shell)
{
	int		shlvl;
	char	*shlvl_new;

	shlvl = get_current_shlvl(shell->envp);
	shlvl_new = ft_itoa(shlvl);
	if (shlvl_new)
	{
		set_env_variable(&shell->envp, "SHLVL", shlvl_new);
		free(shlvl_new);
	}
}
