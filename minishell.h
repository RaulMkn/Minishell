/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:45:12 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/06/23 20:22:37 by ruortiz-         ###   ########.fr       */
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
} t_shell;


#endif
