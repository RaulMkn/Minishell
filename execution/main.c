/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:56:11 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/20 13:27:08 by rmakende         ###   ########.fr       */
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

static void	execute_command(char **cmd, char ***mini_env)
{
	pid_t	pid;

	if (is_builtin(cmd[0]))
		run_builtin(cmd, mini_env, NULL);
	else
	{
		pid = fork();
		if (pid == 0)
			execve(find_command_path(*cmd, *mini_env), cmd, *mini_env);
		else
			wait(NULL);
	}
}

static void	shell_loop(char **mini_env)
{
	char	*line;
	char	**cmd;

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
			free_split(cmd);
			continue ;
		}
		execute_command(cmd, &mini_env);
		free_split(cmd);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**mini_env;

	(void)argc;
	(void)argv;
	mini_env = clone_env(envp);
	shell_loop(mini_env);
	free_split(mini_env);
	rl_clear_history();
	return (0);
}
