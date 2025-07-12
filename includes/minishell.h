/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:02:29 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/11 23:53:44 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

char				*find_command_path(char *cmd, char **envp);
int					run_builtin(char **argv, char ***env);
int					is_builtin(const char *cmd);
char				**clone_env(char **envp);
int					builtin_echo(char **argv);
int					builtin_cd(char **argv, char ***env);
int					builtin_pwd(void);
int					builtin_export(char **argv, char ***env);
int					builtin_unset(char **argv, char ***env);
int					builtin_exit();
int					builtin_env(char **env);
int					replace_env(char **env, const char *key,
						const char *new_var);
int					append_env(char ***env, const char *new_var);
int					is_valid_identifier(const char *str);
int					get_env_index(char **env, const char *key);
void				remove_env_entry(char ***env, int idx);

typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	int				append;
	int				pipe[2];
	struct s_cmd	*next;
}					t_cmd;
#endif