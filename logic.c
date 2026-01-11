#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <termios.h>
#include <ctype.h>

typedef enum e_token_type
{
    TOK_WORD,
    TOK_PIPE,
    TOK_REDIR_IN,
    TOK_REDIR_OUT,
    TOK_REDIR_APPEND,
    TOK_BACKGROUND
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

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
    int len = (int)strlen(cmd);

    while (i < len)
    {
        if (cmd[i] == ' ' || cmd[i] == '\t')
        {
            i++;
            continue;
        }

        if (cmd[i] == '>' && (i + 1) < len && cmd[i + 1] == '>')
        {
            add_token(&tokens, new_token(TOK_REDIR_APPEND, NULL));
            i += 2;
            continue;
        }
        else if (cmd[i] == '>')
        {
            add_token(&tokens, new_token(TOK_REDIR_OUT, NULL));
            i++;
            continue;
        }
        else if (cmd[i] == '<')
        {
            add_token(&tokens, new_token(TOK_REDIR_IN, NULL));
            i++;
            continue;
        }
        else if (cmd[i] == '|')
        {
            add_token(&tokens, new_token(TOK_PIPE, NULL));
            i++;
            continue;
        }
        else if (cmd[i] == '&')
        {
            add_token(&tokens, new_token(TOK_BACKGROUND, NULL));
            i++;
            continue;
        }
        else
        {
            int start = i;

            while (i < len && cmd[i] != ' ' && cmd[i] != '\t'
                   && cmd[i] != '<' && cmd[i] != '>'
                   && cmd[i] != '|' && cmd[i] != '&')
            {
                i++;
            }

            int size = i - start;
            char *word = strndup(cmd + start, (size_t)size);
            add_token(&tokens, new_token(TOK_WORD, word));
            free(word);
            continue;
        }
    }

    return tokens;
}

char *reader()
{
    char *line = NULL;
    size_t len = 0;

    ssize_t nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        free(line);
        return NULL;
    }

    line[strcspn(line, "\n")] = 0;
    return line;
}

void print_tokens(t_token *t)
{
    while (t)
    {
        if (t->type == TOK_WORD)
            printf("WORD(%s)\n", t->value ? t->value : "");
        else if (t->type == TOK_PIPE)
            printf("PIPE\n");
        else if (t->type == TOK_REDIR_IN)
            printf("REDIR_IN\n");
        else if (t->type == TOK_REDIR_OUT)
            printf("REDIR_OUT\n");
        else if (t->type == TOK_REDIR_APPEND)
            printf("REDIR_APPEND\n");
        else if (t->type == TOK_BACKGROUND)
            printf("BACKGROUND\n");

        t = t->next;
    }
}

void free_tokens(t_token *t)
{
    while (t)
    {
        t_token *n = t->next;
        free(t->value);
        free(t);
        t = n;
    }
}

int main()
{
    char *raw = reader();
    if (!raw)
        return 0;

    t_token *toks = tokenize(raw);
    print_tokens(toks);

    free_tokens(toks);
    free(raw);

    return 0;
}
