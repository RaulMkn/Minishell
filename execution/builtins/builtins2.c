/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 18:54:31 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/16 20:05:55 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;
	char *tmp;

	i = 1;
	newline = 1;
	
	while (argv[i] && is_n_option(argv[i]))
	{
		newline = 0;
		i++;
	}
	
	while (argv[i])
	{
		// Comprobar si el argumento está entre comillas simples
		tmp = argv[i];
		
		// Si el argumento original empezaba con comilla simple, mostrar literal $VAR en lugar de expandir
		if (tmp[0] == '\'' && tmp[ft_strlen(tmp) - 1] == '\'')
		{
			// Mostrar el contenido literalmente (entre comillas)
			char *content = ft_substr(tmp, 1, ft_strlen(tmp) - 2);
			ft_printf("%s", content);
			free(content);
		}
		else if (ft_strchr(tmp, '\''))
		{
			// Para casos mixtos como '$VAR'$a, necesitamos preservar la parte entre comillas
			// pero aún permitir la expansión de las partes sin comillas
			ft_printf("%s", tmp);
		}
		else
		{
			// Comportamiento normal - ya expandido
			ft_printf("%s", tmp);
		}
		
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

__attribute__((noreturn)) void free_and_exit(int exit_status)
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
