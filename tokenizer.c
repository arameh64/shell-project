#include "includes.h"

t_token *new_token(t_token_type type, char *value)
{
    t_token *tok = malloc(sizeof(t_token));
    if (!tok)
        return NULL;

    tok->type = type;
    tok->value = value ? strdup(value) : NULL;
    tok->next = NULL;
    return tok;
}

void add_token(t_token **head, t_token *newt)
{
    t_token *tmp;

    if (!newt)
        return;

    if (!*head)
    {
        *head = newt;
        return;
    }

    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;

    tmp->next = newt;
}

t_token *tokenize(char *cmd)
{
    t_token *tokens = NULL;
    int i = 0;
    int len = strlen(cmd);

    while (i < len)
    {
        if (cmd[i] == ' ' || cmd[i] == '\t')
        {
            i++;
            continue;
        }

        if (cmd[i] == '>' && i + 1 < len && cmd[i + 1] == '>')
        {
            add_token(&tokens, new_token(TOK_REDIR_APPEND, NULL));
            i += 2;
        }
        else if (cmd[i] == '>')
        {
            add_token(&tokens, new_token(TOK_REDIR_OUT, NULL));
            i++;
        }
        else if (cmd[i] == '<')
        {
            add_token(&tokens, new_token(TOK_REDIR_IN, NULL));
            i++;
        }
        else if (cmd[i] == '|')
        {
            add_token(&tokens, new_token(TOK_PIPE, NULL));
            i++;
        }
        else if (cmd[i] == '&')
        {
            add_token(&tokens, new_token(TOK_BACKGROUND, NULL));
            i++;
        }
        else
        {
            int start = i;
            while (i < len && cmd[i] != ' ' && cmd[i] != '\t'
                   && cmd[i] != '<' && cmd[i] != '>'
                   && cmd[i] != '|' && cmd[i] != '&')
                i++;

            char *word = strndup(cmd + start, i - start);
            add_token(&tokens, new_token(TOK_WORD, word));
            free(word);
        }
    }

    return tokens;
}
