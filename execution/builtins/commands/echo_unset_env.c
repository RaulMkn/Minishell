/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 18:54:31 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 13:28:18 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	print_echo_argument(char *arg)
{
	ft_printf("%s", arg);
}

static int	skip_n_flags(char **argv, int *i)
{
	int	newline;

	newline = 1;
	while (argv[*i] && is_n_option(argv[*i]))
	{
		newline = 0;
		(*i)++;
	}
	return (newline);
}

int	builtin_echo(char **argv)
{
	int		i;
	int		newline;

	if (argv[1] && !argv[2] && ft_strlen(argv[1]) == 1 && argv[1][0] == '\'')
	{
		ft_printf("\n");
		return (0);
	}
	i = 1;
	newline = skip_n_flags(argv, &i);
	while (argv[i])
	{
		print_echo_argument(argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

int	builtin_unset(char **argv, char ***env)
{
	int	i;
	int	idx;
	int	error_occurred;

	i = 1;
	error_occurred = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(2, "minishell: unset: `", 20);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "`: not a valid identifier\n", 27);
			error_occurred = 1;
		}
		else
		{
			idx = get_env_index(*env, argv[i]);
			if (idx != -1)
				remove_env_entry(env, idx);
		}
		i++;
	}
	return (error_occurred);
}

int	builtin_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
