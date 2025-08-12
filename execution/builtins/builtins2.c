/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 18:54:31 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/12 21:34:03 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
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

__attribute__((noreturn))
void	free_and_exit(char **argv, char **mini_env, char *line, int exit_status)
{
	free_split(argv);
	free_split(mini_env);
	free(line);
	rl_clear_history();
	exit(exit_status);
}

int	builtin_exit(char ***mini_env, char *line, char **argv)
{
	long	exit_code;
	int		final_code;

	if (argv[1] && argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	ft_printf("exit\n");
	if (!argv[1])
		free_and_exit(argv, *mini_env, line, 0);
	if (!is_valid_number(argv[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		free_and_exit(argv, *mini_env, line, 2);
	}
	if (argv[2])
		return (write(2, "minishell: exit: too many arguments\n", 37), 1);
	exit_code = ft_atol(argv[1]);
	final_code = (int)(exit_code % 256);
	if (final_code < 0)
		final_code += 256;
	free_and_exit(argv, *mini_env, line, final_code);
}
