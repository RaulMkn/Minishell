#include "minishell.h"

static int check_redirection_file(t_token *token)
{
    if (!token || !token->next || token->next->type != TOKEN_WORD)
        return (0);
    return (1);
}

static int check_pipe_syntax(t_token *token)
{
    if (!token->next || token->next->type == TOKEN_PIPE)
        return (0);
    if (token == NULL || token->type == TOKEN_PIPE)
        return (0);
    return (1);
}

int is_valid_operator_sequence(t_token *tokens)
{
    t_token *curr;

    curr = tokens;
    while (curr)
    {
        if (curr->type == TOKEN_PIPE && !check_pipe_syntax(curr))
            return (0);
        if ((curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_REDIR_OUT ||
            curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC) &&
            !check_redirection_file(curr))
            return (0);
        curr = curr->next;
    }
    return (1);
}
