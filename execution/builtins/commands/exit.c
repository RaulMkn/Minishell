/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:30:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/17 15:30:00 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

__attribute__((noreturn))
void	free_and_exit(int exit_status)
{
	rl_clear_history();
	exit(exit_status);
}

int	builtin_exit(char ***mini_env, char *line, char **argv)
{
	long	exit_code;
	int		final_code;

	(void)mini_env;
	(void)line;
	if (argv[1] && argv[2])
	{
		ft_printf("exit\n");
		return (write(2, "too many arguments\n", 19), 1);
	}
	ft_printf("exit\n");
	if (!argv[1])
		free_and_exit(0);
	if (!is_valid_number(argv[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		free_and_exit(2);
	}
	exit_code = ft_atol(argv[1]);
	final_code = (int)(exit_code % 256);
	if (final_code < 0)
		final_code += 256;
	free_and_exit(final_code);
}
