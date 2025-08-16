/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:03:17 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/16 18:08:44 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_cd_dotdot(char ***env)
{
	char	*current_pwd;
	char	*parent_dir;
	char	*last_slash;
	char	*cwd_check;

	// Verificar si el directorio actual existe
	cwd_check = getcwd(NULL, 0);
	if (!cwd_check)
	{
		// El directorio actual fue eliminado, usar PWD lógico
		current_pwd = get_env_value(*env, "PWD");
		if (!current_pwd)
		{
			write(2, "minishell: cd: ..: No such file or directory\n", 46);
			return (1);
		}
		// Construir directorio padre desde PWD
		parent_dir = ft_strdup(current_pwd);
		last_slash = ft_strrchr(parent_dir, '/');
		if (last_slash && last_slash != parent_dir)
		{
			*last_slash = '\0'; // Truncar para obtener el padre
		}
		else if (last_slash == parent_dir)
		{
			parent_dir[1] = '\0'; // Estamos en raíz, padre es "/"
		}
		// Intentar cambiar al directorio padre
		if (chdir(parent_dir) == -1)
		{
			write(2, "minishell: cd: ..: No such file or directory\n", 46);
			free(parent_dir);
			return (1);
		}
		free(parent_dir);
	}
	else
	{
		// Directorio actual existe, cd .. normal
		free(cwd_check);
		if (chdir("..") == -1)
			return (perror("minishell: cd"), 1);
	}
	return (0);
}

int	builtin_cd(char **argv, char ***env)
{
	char	*home;
	char	*current_pwd;
	char	*new_pwd;
	char	*temp_path;
	char	*oldpwd;
	char	*parent;
	char	*last_slash;
	char	*with_slash;

	if (argv[1] && argv[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	// Guardar PWD actual como futuro OLDPWD
	current_pwd = get_env_value(*env, "PWD");
	if (!current_pwd)
	{
		current_pwd = getcwd(NULL, 0);
		if (current_pwd)
		{
			set_env_variable(env, "PWD", current_pwd);
			free(current_pwd);
			current_pwd = get_env_value(*env, "PWD");
		}
	}
	if (argv[1] == NULL || (argv[1][0] == '\0'))
	{
		home = get_env_value(*env, "HOME");
		if (!home)
			return (write(2, "minishell: cd: HOME not set\n", 29), 1);
		if (chdir(home) == -1)
			return (perror("minishell: cd"), 1);
	}
	else if (!ft_strcmp(argv[1], "-"))
	{
		oldpwd = get_env_value(*env, "OLDPWD");
		if (!oldpwd)
			return (write(2, "minishell: cd: OLDPWD not set\n", 31), 1);
		if (chdir(oldpwd) == -1)
			return (perror("minishell: cd"), 1);
		ft_printf("%s\n", oldpwd);
	}
	else if (!ft_strcmp(argv[1], ".."))
	{
		// Manejo especial para cd ..
		if (handle_cd_dotdot(env) != 0)
			return (1);
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			perror("minishell: cd");
			return (1);
		}
	}
	// Actualizar OLDPWD con el PWD anterior
	if (current_pwd)
		set_env_variable(env, "OLDPWD", current_pwd);
	// Actualizar PWD con el nuevo directorio
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		set_env_variable(env, "PWD", new_pwd);
		free(new_pwd);
	}
	else
	{
		// getcwd falló, usar navegación lógica
		if (argv[1] && current_pwd)
		{
			if (!ft_strcmp(argv[1], ".."))
			{
				parent = ft_strdup(current_pwd);
				last_slash = ft_strrchr(parent, '/');
				if (last_slash && last_slash != parent)
					*last_slash = '\0';
				else if (last_slash == parent)
					parent[1] = '\0';
				set_env_variable(env, "PWD", parent);
				free(parent);
			}
			else if (argv[1][0] == '/')
			{
				set_env_variable(env, "PWD", argv[1]);
			}
			else if (!ft_strcmp(argv[1], "-"))
			{
				oldpwd = get_env_value(*env, "OLDPWD");
				if (oldpwd)
					set_env_variable(env, "PWD", oldpwd);
			}
			else
			{
				// Path relativo - construir usando ft_strjoin
				if (current_pwd[ft_strlen(current_pwd) - 1] == '/')
					temp_path = ft_strjoin(current_pwd, argv[1]);
				else
				{
					with_slash = ft_strjoin(current_pwd, "/");
					temp_path = ft_strjoin(with_slash, argv[1]);
					free(with_slash);
				}
				if (temp_path)
				{
					set_env_variable(env, "PWD", temp_path);
					free(temp_path);
				}
			}
		}
	}
	return (0);
}

// ...existing code...

int	builtin_pwd(char **env)
{
    char	*cwd;
    char	*env_pwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        env_pwd = get_env_value(env, "PWD");
        if (env_pwd)
            return (ft_printf("%s\n", env_pwd), 0);
        else
        {
            write(2, "pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 108);
            return (1);
        }
    }
    env_pwd = get_env_value(env, "PWD");
    if (!env_pwd || ft_strcmp(env_pwd, cwd) != 0)
        set_env_variable(&env, "PWD", cwd);
    ft_printf("%s\n", cwd);
    free(cwd);
    return (0);
}

// ...existing code...

static int	export_display_all(char **env)
{
	int		j;
	char	*eq;

	j = 0;
	while (env[j])
	{
		eq = ft_strchr(env[j], '=');
		if (eq)
		{
			write(1, "declare -x ", 11);
			write(1, env[j], eq - env[j]);
			write(1, "=\"", 2);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		j++;
	}
	return (0);
}

static int	export_process_args(char **argv, char ***env, int *error_occurred)
{
	int		i;
	char	*eq;
	char	*key;

	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (!is_valid_identifier(argv[i]))
		{
			write(2, "minishell: export: not a valid identifier\n", 42);
			*error_occurred = 1;
		}
		else if (eq)
		{
			key = ft_substr(argv[i], 0, eq - argv[i]);
			if (get_env_index(*env, key) >= 0)
				replace_env(*env, key, argv[i]);
			else
				append_env(env, argv[i]);
			free(key);
		}
		i++;
	}
	return (*error_occurred);
}

int	builtin_export(char **argv, char ***env)
{
	int	error_occurred;

	error_occurred = 0;
	if (!argv[1])
		return (export_display_all(*env));
	return (export_process_args(argv, env, &error_occurred));
}
