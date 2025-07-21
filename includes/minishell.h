/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:34:05 by rmakende          #+#    #+#             */
/*   Updated: 2025/07/22 01:51:41 by rmakende         ###   ########.fr       */
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
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}						t_token_type;
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}						t_redir_type;
typedef struct s_redir
{
	t_redir_type		type;
	char				*file;
	struct s_redir		*next;
}						t_redir;

typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}						t_quote_state;

typedef enum e_error_type
{
	ERROR_NONE,
	ERROR_SYNTAX,
	ERROR_MEMORY,
	ERROR_INVALID_OPERATOR
}						t_error_type;
typedef struct s_lexer_state
{
	t_quote_state		quote_state;
	t_error_type		error;
	char				*error_msg;
}						t_lexer_state;
typedef struct s_command
{
	char				**argv;
	t_redir				*redir;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	t_command			*cmd_list;
	char				**envp;
	int					last_status;
	t_lexer_state		lexer_state;
}						t_shell;

typedef struct s_cmd
{
	char				**argv;
	char				*infile;
	char				*outfile;
	int					append;
	struct s_cmd		*next;
}						t_cmd;

char					**parse_argv(t_token **tokens);
t_redir					*parse_redirections(t_token **tokens);
t_command				*parse_tokens(t_token *tokens);
char					*expand_variables(char *str, char **env,
							int last_status);
void					clear_command(t_command *cmd);
void					set_error(t_lexer_state *state, t_error_type error,
							char *msg);
t_token					*tokenize_input(char *in, t_shell *shell);
int						is_valid_operator_sequence(t_token *tokens);
void					clear_tokens(t_token **tokens);
char					*find_command_path(char *cmd, char **envp);
int						run_builtin(char **argv, char ***env, char *line);
int						is_builtin(const char *cmd);
char					**clone_env(char **envp);
int						builtin_echo(char **argv);
int						builtin_cd(char **argv, char ***env);
int						builtin_pwd(void);
int						builtin_export(char **argv, char ***env);
int						builtin_unset(char **argv, char ***env);
int						builtin_exit(char ***mini_env, char *line, char **argv);
int						builtin_env(char **env);
int						replace_env(char **env, const char *key,
							const char *new_var);
int						append_env(char ***env, const char *new_var);
int						is_valid_identifier(const char *str);
int						get_env_index(char **env, const char *key);
char					*get_env_value(char **env, const char *key);
void					remove_env_entry(char ***env, int idx);
int						is_n_option(const char *str);
t_token					*create_token(t_token_type type, char *value);
void					token_add_back(t_token **tokens, t_token *new);
t_token					*create_operator_token(char c, int len);
char					*ft_strjoin_char(char *str, char c);
void					handle_buffer_token(t_token **tokens, char **buffer);
void					handle_whitespace(t_token **tokens, char **buffer,
							size_t *i, char *input);
void					handle_operator(t_token **tokens, char **buffer,
							size_t *i, char *input);
void					handle_quote_char(char **buffer, char c, size_t *i,
							t_shell *shell);
void					handle_regular_char(char **buffer, char c, size_t *i,
							t_shell *shell);
void					handle_final_buffer(t_token **tokens, char **buffer,
							t_shell *shell);
int						is_valid_operator_syntax(char *input, size_t i);
int						exec_dispatch(t_cmd *cmd, char ***mini_env);

// Execution functions
void					init_shell(t_shell *shell, char **envp);
void					cleanup_shell(t_shell *shell);
void					shell_loop(t_shell *shell);
void					execute_pipeline(t_command *cmd_list, char ***mini_env);
void					handle_redirections(t_redir *redirs);
void					execute_child_process(t_command *cmd, char ***mini_env,
							int prev_fd, int *pipe_fd);
void					handle_parent_process(int *prev_fd, int *pipe_fd,
							t_command *current);

// Pipe execution functions
int						exec_single(t_cmd *cmd, char ***mini_env);
int						exec_pipeline(t_cmd *cmd, char ***mini_env);
void					setup_child_input(t_cmd *curr, int in_fd);
void					setup_child_output(t_cmd *curr, int *pipe_fd);
void					execute_child_command(t_cmd *curr, char ***mini_env);
void					handle_child_process(t_cmd *curr, char ***mini_env,
							int in_fd, int *pipe_fd);
int						handle_pipe_parent_process(int in_fd, int *pipe_fd,
							t_cmd *curr);
void					redirect_input(const char *infile);
void					redirect_output(const char *outfile, int append);
void					perror_exit(char *msg);

#endif