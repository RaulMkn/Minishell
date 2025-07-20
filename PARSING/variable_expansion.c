#include "minishell.h"

char *expand_variables(char *str, char **env, int last_status)
{
    char *result;
    int i;
    
    if (!str || !env)
        return (NULL);
        
    if (str[0] == '$' && str[1] == '?')
        return (ft_itoa(last_status));
        
    if (str[0] == '$')
    {
        i = 0;
        while (env[i])
        {
            if (ft_strncmp(str + 1, env[i], ft_strchr(env[i], '=') - env[i]) == 0)
                return (ft_strdup(ft_strchr(env[i], '=') + 1));
            i++;
        }
    }
    return (ft_strdup(str));
}

void clear_command(t_command *cmd)
{
    int i;
    t_redir *tmp;

    if (!cmd)
        return;
    if (cmd->argv)
    {
        i = 0;
        while (cmd->argv[i])
            free(cmd->argv[i++]);
        free(cmd->argv);
    }
    while (cmd->redir)
    {
        tmp = cmd->redir->next;
        free(cmd->redir->file);
        free(cmd->redir);
        cmd->redir = tmp;
    }
    free(cmd);
}

void set_error(t_lexer_state *state, t_error_type error, char *msg)
{
    if (state->error_msg)
        free(state->error_msg);
    state->error = error;
    state->error_msg = ft_strdup(msg);
}
