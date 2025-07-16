/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:42:17 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/07/13 20:39:48 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void token_add_back(t_token **tokens, t_token *new)
{
    t_token *temp;

    if (!tokens || !new)
        return ;
    if (!*tokens)
    {
        *tokens = new;
        return ;
    }
    temp = *tokens;
    while (temp->next)
        temp = temp->next;
    temp->next = new;
}
char *ft_strjoin_char(char *str, char c)
{
    char    *new;
    int     i;
    int     len;

    len = 0;
    if (str)
        len = strlen(str);
    if (!(new = malloc(sizeof(char) * (len + 2))))
    {
        free(str);
        return (NULL);
    }
    i = 0;
    if (str)
    {
        while (str[i])
        {
            new[i] = str[i];
            i++;
        }
        free(str);
    }
    new[i] = c;
    new[i + 1] = '\0';
    return (new);
}

t_token *create_token(t_token_type type, char *value)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->type = type;
    new->value = ft_strdup(value);
    if (!new->value)
    {
        free(new);
        return (NULL);
    }
    new->next = NULL;
    return (new);
}

t_token *create_operator_token(char c, int len)
{
    char op[3];
    t_token_type type;

    op[0] = c;
    if (len == 2)
        op[1] = c;
    else
        op[1] = '\0';
    op[2] = '\0';
    if (c == '|')
        type = TOKEN_PIPE;
    else if (c == '<' && len == 1)
        type = TOKEN_REDIR_IN;
    else if (c == '>' && len == 1)
        type = TOKEN_REDIR_OUT;
    else if (c == '>' && len == 2)
        type = TOKEN_APPEND;
    else if (c == '<' && len == 2)
        type = TOKEN_HEREDOC;
    return (create_token(type, op));
}
static size_t	handle_whitespaces(const char *input, size_t i)
{
    while (input[i] && (input[i] == ' ' || input[i] == '\t' || 
           input[i] == '\n' || input[i] == '\v' || 
           input[i] == '\f' || input[i] == '\r'))
        i++;
    return (i);
}
void	handle_whitespace(t_token **tokens, char **buffer, size_t *i, char *input)
{
    char *temp;

    if (*buffer && **buffer != '\0')
    {
        if (!(temp = ft_strdup(*buffer)))
        {
            free(*buffer);
            *buffer = NULL;
            return ;
        }
        token_add_back(tokens, create_token(TOKEN_WORD, temp));
        free(temp);
        free(*buffer);
        *buffer = NULL;
    }
    *i = handle_whitespaces(input, *i);
}

void handle_buffer_token(t_token **tokens, char **buffer)
{
    char *temp;
    t_token *new_token;

    if (!(*buffer && **buffer != '\0'))
        return ;
    if (!(temp = ft_strdup(*buffer)))
    {
        free(*buffer);
        *buffer = NULL;
        return ;
    }
    if (!(new_token = create_token(TOKEN_WORD, temp)))
    {
        free(temp);
        free(*buffer);
        *buffer = NULL;
        return ;
    }
    token_add_back(tokens, new_token);
    free(temp);
    free(*buffer);
    *buffer = NULL;
}

static int	is_valid_operator_syntax(char *input, size_t i)
{
	if (!input[i + 1] || !input[i + 2])
		return (1);
	if ((input[i] == '>' || input[i] == '<') && 
		(input[i + 1] == '>' || input[i + 1] == '<'))
		return (0);
	if (input[i] == '|' && input[i + 1] == '|')
		return (0);
	return (1);
}

static void	handle_operator_error(t_token **tokens, char **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	while (*tokens)
	{
		free((*tokens)->value);
		free(*tokens);
		*tokens = (*tokens)->next;
	}
}

static int is_valid_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

static int check_adjacent_operators(char *input, size_t i)
{
    if (!input[i])
        return (1);
    if (is_valid_operator_char(input[i]))
    {
        if (input[i] == '|' && input[i + 1] == '|')
            return (0);
        if ((input[i] == '<' && (input[i + 1] == '>' || input[i + 1] == '|')) ||
            (input[i] == '>' && (input[i + 1] == '<' || input[i + 1] == '|')))
            return (0);
    }
    return (1);
}

static void handle_quotes(char c, t_lexer_state *state)
{
    if (c == '\'' && state->quote_state == QUOTE_NONE)
        state->quote_state = QUOTE_SINGLE;
    else if (c == '\'' && state->quote_state == QUOTE_SINGLE)
        state->quote_state = QUOTE_NONE;
    else if (c == '\"' && state->quote_state == QUOTE_NONE)
        state->quote_state = QUOTE_DOUBLE;
    else if (c == '\"' && state->quote_state == QUOTE_DOUBLE)
        state->quote_state = QUOTE_NONE;
}

void	handle_operator(t_token **tokens, char **buffer, size_t *i, char *input)
{
    if (!check_adjacent_operators(input, *i))
    {
        handle_operator_error(tokens, buffer);
        return ;
    }
    handle_buffer_token(tokens, buffer);
    if ((input[*i] == '>' || input[*i] == '<') && 
        input[*i + 1] == input[*i] && input[*i + 1] != ' ')
    {
        token_add_back(tokens, create_operator_token(input[*i], 2));
        (*i) += 2;
    }
    else
    {
        token_add_back(tokens, create_operator_token(input[*i], 1));
        (*i)++;
    }
}

static void handle_word(t_token **tokens, char *word, t_shell *shell)
{
    char *expanded;
    
    expanded = expand_variables(word, shell->envp, shell->last_status);
    if (!expanded)
    {
        set_error(&shell->lexer_state, ERROR_MEMORY, "Error de memoria al expandir variables");
        return;
    }
    
    token_add_back(tokens, create_token(TOKEN_WORD, expanded));
    free(expanded);
}

t_token *tokenize_input(char *in, t_shell *shell)
{
    t_token *tokens;
    size_t i;
    char *buffer;

    tokens = NULL;
    i = 0;
    buffer = NULL;
    while (in[i])
    {
        if ((in[i] >= 9 && in[i] <= 13) || in[i] == 32)
            handle_whitespace(&tokens, &buffer, &i, in);
        else if (in[i] == '|' || in[i] == '<' || in[i] == '>')
            handle_operator(&tokens, &buffer, &i, in);
        else if (!(buffer = ft_strjoin_char(buffer, in[i])))
            return (NULL);
        else
            i++;
    }
    if (buffer)
    {
        if (*buffer != '\0')
            handle_word(&tokens, buffer, shell);
        free(buffer);
    }
    return (tokens);
}

