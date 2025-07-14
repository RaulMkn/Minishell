/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:56:11 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/14 21:55:29 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ELIMINAR RUBEN PERRA ESTO TE TOCA HACERLO A TI
char	**parse_line(char *line)
{
	char	**tokens;
	int		i;
	char	*token;

	tokens = calloc(10, sizeof(char *));
	i = 0;
	token = strtok(line, " ");
	while (token)
	{
		tokens[i++] = ft_strdup(token);
		token = strtok(NULL, " ");
	}
	tokens[i] = NULL;
	return (tokens);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**cmd;
	pid_t	pid;
	char	**mini_env;

	mini_env = clone_env(envp);
	(void)argc;
	(void)argv;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		cmd = parse_line(line);
		if (!cmd || !cmd[0])
		{
			free(line);
			continue ;
		}
		if (is_builtin(cmd[0]))
			run_builtin(cmd, &mini_env, &line);
		else
		{
			pid = fork();
			if (pid == 0)
				execve(find_command_path(*cmd, mini_env), cmd, mini_env);
			else
				wait(NULL);
		}
	}
	return (0);
}
