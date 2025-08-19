/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:34:05 by rmakende          #+#    #+#             */
/*   Updated: 2025/08/18 21:23:52 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// ========================================================================== //
//                                 INCLUDES                                   //
// ========================================================================== //

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// ========================================================================== //
//                                 ENUMS                                      //
// ========================================================================== //

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}							t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}							t_redir_type;

typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}							t_quote_state;

typedef enum e_error_type
{
	ERROR_NONE,
	ERROR_SYNTAX,
	ERROR_MEMORY,
	ERROR_INVALID_OPERATOR
}							t_error_type;

// ========================================================================== //
//                               STRUCTURES                                   //
// ========================================================================== //

typedef struct s_signal_state
{
	volatile sig_atomic_t	signal_received;
	volatile sig_atomic_t	in_heredoc;
}							t_signal_state;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	struct s_token			*next;
}							t_token;

typedef struct s_redir
{
	t_redir_type			type;
	char					*file;
	struct s_redir			*next;
}							t_redir;

typedef struct s_lexer_state
{
	t_quote_state			quote_state;
	t_error_type			error;
	char					*error_msg;
}							t_lexer_state;

typedef struct s_command
{
	char					**argv;
	t_redir					*redir;
	struct s_command		*next;
}							t_command;

typedef struct s_pipe_data
{
	int						prev_fd;
	int						*pipe_fd;
}							t_pipe_data;

typedef struct s_shell
{
	t_command				*cmd_list;
	char					**envp;
	int						last_status;
	t_lexer_state			lexer_state;
}							t_shell;

typedef struct s_tokenizer_ctx
{
	t_token					**tokens;
	char					**buffer;
	size_t					*i;
	char					*input;
}							t_tokenizer_ctx;

typedef struct s_cmd
{
	char					**argv;
	char					*infile;
	char					*outfile;
	int						append;
	struct s_cmd			*next;
}							t_cmd;

typedef struct s_expansion_context
{
	char					**env;
	int						last_status;
	int						inside_single_quotes;
}							t_expansion_context;

// ========================================================================== //
//                           PARSING FUNCTIONS                               //
// ========================================================================== //

/* Token parsing */
char						**parse_argv(t_token **tokens);
t_redir						*parse_redirections(t_token **tokens);
char						**pars_argv_redirections(t_token **tokens);
t_redir						*parse_redirections_mixed(t_token **tokens);
t_command					*parse_command_unified(t_token **tokens);
t_command					*parse_tokens(t_token *tokens);
t_token						*concatenate_consecutive_tokens(t_token *tokens);
int							handle_pipe_token(t_token **curr_token,
								t_command *current_cmd);
int							process_word_token(t_token **current, char ***argv,
								int *argc);
int							process_redirection_token(t_token **current,
								t_redir **redir_list);

/* Variable expansion */
char						*expand_variables(char *str, char **env,
								int last_status, int len);
char						*expand_complex_variables(char *str, char **env,
								int last_status);
char						*handle_quoted_expansion(char *str, char **env,
								int last_status);
char						*handle_pid_expansion_ext(char **result, int *i);
char						*handle_variable_expansion_ext(char *str,
								char **result, int *i,
								t_expansion_context *ctx);
char						*process_character_expansion(char *str,
								char **result, int *i,
								t_expansion_context *ctx);
void						expand_and_filter_tokens(t_token **tokens,
								t_shell *shell);
char						*get_var_value(char *var_name, char **env,
								int last_status);
int							get_var_name_length(char *str, int start);
t_token						*remove_empty_token(t_token **tokens, t_token *prev,
								t_token *current);
void						update_token_value(t_token *current,
								char *expanded_value);

/* Lexer */
t_token						*tokenize_input(char *in, t_shell *shell);
t_token						*create_token(t_token_type type, char *value);
void						token_add_back(t_token **tokens, t_token *new);
t_token						*create_operator_token(char c, int len);
char						*remove_quotes(const char *str);
void						handle_buffer_token(t_token **tokens,
								char **buffer);
void						handle_whitespace(t_token **tokens, char **buffer,
								size_t *i, char *input);
void						handle_operator(t_token **tokens, char **buffer,
								size_t *i, char *input);
void						handle_quote_char(char **buffer, char c, size_t *i,
								t_shell *shell);
void						handle_regular_char(char **buffer, char c,
								size_t *i, t_shell *shell);
void						handle_final_buffer(t_token **tokens, char **buffer,
								t_shell *shell);
void						init_tokenizer_context(t_tokenizer_ctx *ctx,
								t_token **tokens, char **buffer, size_t *i);
void						set_tokenizer_input(t_tokenizer_ctx *ctx,
								char *input);
t_token						*finalize_tokenization(t_shell *shell,
								char **buffer, t_token **tokens);
int							handle_forbidden_sequence_error(char *input,
								size_t i, t_shell *shell, t_token **tokens);
int							is_forbidden_sequence(char *input, size_t i,
								t_shell *shell);
int							handle_unclosed_quotes(t_shell *shell,
								char **buffer);

/* Token validation and utils */
int							is_valid_operator_sequence(t_token *tokens);
int							is_valid_operator_syntax(char *input, size_t i);
void						clear_tokens(t_token **tokens);
void						c_redir(t_redir *redir);
void						clear_command(t_command *cmd);
void						set_error(t_lexer_state *state, t_error_type error,
								char *msg);
void						copy_string_chars(char *new, char *str, int len);
void						create_word_token(t_token **tokens, char **buffer);
size_t						handle_whitespaces(const char *input, size_t i);
void						remove_quotes_copy(const char *str, char *res);
int							is_redirection_token(t_token_type type);
t_redir						*create_redirection_tn(t_token *redir_token,
								t_token *file_token);
void						add_redir_to_list(t_redir **redir_list,
								t_redir *new_redir);

// ========================================================================== //
//                          EXECUTION FUNCTIONS                              //
// ========================================================================== //

/* Core shell functions */
void						init_shell(t_shell *shell, char **envp);
void						cleanup_shell(t_shell *shell);
void						shell_loop(t_shell *shell);

/* Command execution */
int							execute_pipeline(t_command *cmd_list,
								char ***mini_env, t_shell *shell);
void						execute_child_process(t_command *cmd,
								char ***mini_env, t_pipe_data *p_data,
								t_shell *shell);
void						handle_parent_process(int *prev_fd, int *pipe_fd,
								t_command *current);
int							exec_dispatch(t_cmd *cmd, char ***mini_env);
int							exec_single(t_cmd *cmd, char ***mini_env);
int							exec_pipeline(t_cmd *cmd, char ***mini_env);
char						*find_command_path(char *cmd, char **envp);
int							validate_command_args(t_command *cmd);
int							execute_parent_builtin(t_command *cmd,
								char ***mini_env, t_shell *shell);
int							check_command_path(char *command_path,
								char *cmd_name);
int							check_file_permissions(char *command_path,
								char *cmd_name);

/* Pipeline execution */
void						setup_child_input(t_cmd *curr, int in_fd);
void						setup_child_output(t_cmd *curr, int *pipe_fd);
void						execute_child_command(t_cmd *curr,
								char ***mini_env);
void						handle_child_process(t_cmd *curr, char ***mini_env,
								int in_fd, int *pipe_fd);
int							handle_pipe_parent_process(int in_fd, int *pipe_fd,
								t_cmd *curr);
int							create_pipe_if_needed(t_command *current,
								int *pipe_fd);
int							is_single_command(t_command *cmd_list);
void						setup_input_pipe(int prev_fd);
void						setup_output_pipe(int *pipe_fd);
int							setup_heredocs(t_command *cmd, t_shell *shell,
								int *original_stdin);
void						restore_stdin(int original_stdin);
int							execute_parent_builtin_wrapper(t_command *cmd,
								char ***mini_env, t_shell *shell,
								int original_stdin);
int							execute_builtin_wrapper(t_command *cmd,
								char ***mini_env, t_shell *shell,
								int original_stdin);
int							execute_external_wrapper(t_command *cmd,
								char ***mini_env, t_shell *shell,
								int original_stdin);
pid_t						handle_pipeline_loop(t_command *cmd_list,
								char ***mini_env, t_shell *shell);
int							execute_builtin_in_fork(t_command *cmd,
								char ***mini_env, t_shell *shell);
int							execute_external_command(t_command *cmd,
								char *command_path, char ***mini_env,
								t_shell *shell);

// ========================================================================== //
//                            BUILTIN FUNCTIONS                              //
// ========================================================================== //

/* Builtin command identification and execution */
int							run_builtin(char **argv, char ***env, char *line);
int							is_builtin(const char *cmd);
int							is_parent_builtin(const char *cmd);

/* Builtin commands */
int							builtin_echo(char **argv);
int							builtin_cd(char **argv, char ***env);
int							builtin_pwd(char **env);
int							builtin_export(char **argv, char ***env);
int							builtin_unset(char **argv, char ***env);
int							builtin_exit(char ***mini_env, char *line,
								char **argv);
int							builtin_env(char **env);

/* CD command helpers */
int							handle_cd_dotdot(char ***env);
void						update_pwd_fallback(char **argv, char *current_pwd,
								char ***env);
int							handle_cd_dotdot_simple(void);

/* Environment management */
int							replace_env(char **env, const char *key,
								const char *new_var);
int							append_env(char ***env, const char *new_var);
int							is_valid_identifier(const char *str);
int							get_env_index(char **env, const char *key);
char						*get_env_value(char **env, const char *key);
void						remove_env_entry(char ***env, int idx);
int							set_env_variable(char ***env, const char *key,
								const char *value);

/* Echo command helpers */
int							is_n_option(const char *str);
int							is_valid_number(char *str);

// ========================================================================== //
//                           REDIRECTION FUNCTIONS                           //
// ========================================================================== //

/* Redirection handling */
void						redirect_input(const char *infile);
void						redirect_output(const char *outfile, int append);
int							handle_multiple_redirections(t_redir *redirs,
								t_shell *shell);
int							handle_heredoc(char *delimiter, t_shell *shell);
int							has_heredoc(t_redir *redirs);
int							process_all_heredocs(t_redir *redirs,
								t_shell *shell);

/* Redirection validation */
int							validate_input_file(const char *filename);
int							validate_output_file(const char *filename);
void						find_last_redirections(t_redir *redirs,
								t_redir **last_in, t_redir **last_out,
								t_redir **last_append);
t_redir						*create_effective_redir(t_redir *source,
								t_redir *list);

// ========================================================================== //
//                             SIGNAL FUNCTIONS                              //
// ========================================================================== //

void						setup_interactive_signals(void);
void						setup_execution_signals(void);
t_signal_state				*get_signal_state(void);
void						set_signal_received(int signal);
void						set_heredoc_state(int state);
int							get_signal_received(void);
void						reset_signal_state(void);

// ========================================================================== //
//                             UTILITY FUNCTIONS                             //
// ========================================================================== //

void						perror_exit(char *msg);
void						append_char(char **result, char *str, int *i);

#endif