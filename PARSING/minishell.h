/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:45:12 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/07/13 20:39:48 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "libft/ft_printf.h"
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
typedef enum e_token_type {
    TOKEN_WORD,        // palabras: comandos, args, nombres de archivos
    TOKEN_PIPE,        // |
    TOKEN_REDIR_IN,    // <
    TOKEN_REDIR_OUT,   // >
    TOKEN_APPEND,      // >>
    TOKEN_HEREDOC      // <<
} t_token_type;
typedef struct s_token {
    t_token_type    type;
    char            *value;
    struct s_token  *next;
} t_token;

typedef enum e_redir_type {
    REDIR_IN,      // <
    REDIR_OUT,     // >
    REDIR_APPEND,  // >>
    HEREDOC        // <<
} t_redir_type;
typedef struct s_redir {
    t_redir_type    type;
    char            *file;        // nombre del archivo o delimitador
    struct s_redir  *next;
} t_redir;
typedef struct s_command {
    char            **argv;       // lista de args (argv[0] = comando)
    t_redir         *redir;       // redirecciones asociadas
    struct s_command *next;       // para pipelines (| cmd1 | cmd2)
} t_command;

typedef struct s_shell {
    t_command   *cmd_list;       // comandos a ejecutar
    char        **envp;          // entorno (export, unset)
    int         last_status;     // $? (último código de salida)
    t_lexer_state lexer_state;
} t_shell;

typedef enum e_quote_state {
    QUOTE_NONE,
    QUOTE_SINGLE,
    QUOTE_DOUBLE
} t_quote_state;

typedef enum e_error_type {
    ERROR_NONE,
    ERROR_SYNTAX,
    ERROR_MEMORY,
    ERROR_INVALID_OPERATOR
} t_error_type;

typedef struct s_lexer_state {
    t_quote_state quote_state;
    t_error_type error;
    char *error_msg;
} t_lexer_state;

// Function prototypes
char *expand_variables(char *str, char **env, int last_status);
void clear_command(t_command *cmd);
void set_error(t_lexer_state *state, t_error_type error, char *msg);
t_token *tokenize_input(char *in, t_shell *shell);
int is_valid_operator_sequence(t_token *tokens);
void clear_tokens(t_token **tokens);

#endif
