/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:42:17 by ruortiz-          #+#    #+#             */
/*   Updated: 2025/06/17 20:01:59 by ruortiz-         ###   ########.fr       */
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

void handle_whitespace(t_token **tokens, char **buffer, size_t *i)
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
    (*i)++;
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

void handle_operator(t_token **tokens, char **buffer, size_t *i, char *input)
{
    t_token *new_token;

    handle_buffer_token(tokens, buffer);
    if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
    {
        if (!(new_token = create_operator_token(input[*i], 2)))
            return ;
        token_add_back(tokens, new_token);
        (*i) += 2;
    }
    else
    {
        if (!(new_token = create_operator_token(input[*i], 1)))
            return ;
        token_add_back(tokens, new_token);
        (*i)++;
    }
}

t_token *tokenize_input(char *in)
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
            handle_whitespace(&tokens, &buffer, &i);
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
            token_add_back(&tokens, create_token(TOKEN_WORD, buffer));
        free(buffer);
    }
    return (tokens);
}

