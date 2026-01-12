#include "includes.h"

t_cmd *new_cmd()
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;

    cmd->argv = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    cmd->background = 0;
    cmd->next = NULL;
    return cmd;
}

void add_cmd(t_cmd **head, t_cmd *newc)
{
    t_cmd *tmp;

    if (!newc)
        return;

    if (!*head)
    {
        *head = newc;
        return;
    }

    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;

    tmp->next = newc;
}

void add_arg(t_cmd *cmd, char *word)
{
    int i = 0;
    char **new_argv;

    if (!cmd->argv)
    {
        new_argv = malloc(sizeof(char *) * 2);
        new_argv[0] = strdup(word);
        new_argv[1] = NULL;
        cmd->argv = new_argv;
        return;
    }

    while (cmd->argv[i])
        i++;

    new_argv = malloc(sizeof(char *) * (i + 2));
    for (int j = 0; j < i; j++)
        new_argv[j] = cmd->argv[j];

    new_argv[i] = strdup(word);
    new_argv[i + 1] = NULL;

    free(cmd->argv);
    cmd->argv = new_argv;
}

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *cmds = NULL;
    t_cmd *current = new_cmd();

    while (tokens)
    {
        if (tokens->type == TOK_WORD)
            add_arg(current, tokens->value);
        else if (tokens->type == TOK_REDIR_IN)
        {
            tokens = tokens->next;
            if (tokens && tokens->type == TOK_WORD)
                current->infile = strdup(tokens->value);
        }
        else if (tokens->type == TOK_REDIR_OUT)
        {
            tokens = tokens->next;
            if (tokens && tokens->type == TOK_WORD)
            {
                current->outfile = strdup(tokens->value);
                current->append = 0;
            }
        }
        else if (tokens->type == TOK_REDIR_APPEND)
        {
            tokens = tokens->next;
            if (tokens && tokens->type == TOK_WORD)
            {
                current->outfile = strdup(tokens->value);
                current->append = 1;
            }
        }
        else if (tokens->type == TOK_PIPE)
        {
            add_cmd(&cmds, current);
            current = new_cmd();
        }
        else if (tokens->type == TOK_BACKGROUND)
            current->background = 1;

        tokens = tokens->next;
    }

    add_cmd(&cmds, current);
    return cmds;
}
