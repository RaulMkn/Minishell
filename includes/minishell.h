/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:02:29 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 00:01:23 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <errno.h>
# include <fcntl.h> // Para open()
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_WORD,      // palabras: comandos, args, nombres de archivos
	TOKEN_PIPE,      // |
	TOKEN_REDIR_IN,  // <
	TOKEN_REDIR_OUT, // >
	TOKEN_APPEND,    // >>
	TOKEN_HEREDOC    // <<
}					t_token_type;
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef enum e_redir_type
{
	REDIR_IN,     // <
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	HEREDOC       // <<
}					t_redir_type;
typedef struct s_redir
{
	t_redir_type	type;
	char *file; // nombre del archivo o delimitador
	struct s_redir	*next;
}					t_redir;

typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}					t_quote_state;

typedef enum e_error_type
{
	ERROR_NONE,
	ERROR_SYNTAX,
	ERROR_MEMORY,
	ERROR_INVALID_OPERATOR
}					t_error_type;
typedef struct s_lexer_state
{
	t_quote_state	quote_state;
	t_error_type	error;
	char			*error_msg;
}					t_lexer_state;
typedef struct s_command
{
	char **argv;            // lista de args (argv[0] = comando)
	t_redir *redir;         // redirecciones asociadas
	struct s_command *next; // para pipelines (| cmd1 | cmd2)
}					t_command;

typedef struct s_shell
{
	t_command *cmd_list; // comandos a ejecutar
	char **envp;         // entorno (export, unset)
	int last_status;     // $? (último código de salida)
	t_lexer_state	lexer_state;
}					t_shell;

typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	int				append;
	struct s_cmd	*next;
}					t_cmd;

// Añadir antes de las definiciones de funciones:
char				**parse_argv(t_token **tokens);
t_redir				*parse_redirections(t_token **tokens);
t_command			*parse_tokens(t_token *tokens);

// Function prototypes
char				*expand_variables(char *str, char **env, int last_status);
void				clear_command(t_command *cmd);
void				set_error(t_lexer_state *state, t_error_type error,
						char *msg);
t_token				*tokenize_input(char *in, t_shell *shell);
int					is_valid_operator_sequence(t_token *tokens);
void				clear_tokens(t_token **tokens);
char				*find_command_path(char *cmd, char **envp);
int					run_builtin(char **argv, char ***env, char *line);
int					is_builtin(const char *cmd);
char				**clone_env(char **envp);
int					builtin_echo(char **argv);
int					builtin_cd(char **argv, char ***env);
int					builtin_pwd(void);
int					builtin_export(char **argv, char ***env);
int					builtin_unset(char **argv, char ***env);
int					builtin_exit(char ***mini_env, char *line, char **argv);
int					builtin_env(char **env);
int					replace_env(char **env, const char *key,
						const char *new_var);
int					append_env(char ***env, const char *new_var);
int					is_valid_identifier(const char *str);
int					get_env_index(char **env, const char *key);
char				*get_env_value(char **env, const char *key);
void				remove_env_entry(char ***env, int idx);
int					is_n_option(const char *str);
t_token				*create_token(t_token_type type, char *value);
void				token_add_back(t_token **tokens, t_token *new);
int					exec_dispatch(t_cmd *cmd, char ***mini_env);

#endif