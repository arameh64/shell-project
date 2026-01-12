#include "includes.h"

void execute_commands(t_cmd *cmds, t_history **history)
{
    (void)history;

    printf("[DEBUG] execute_commands called\n");

    while (cmds)
    {
        if (cmds->argv && cmds->argv[0])
            printf("[DEBUG] would execute: %s\n", cmds->argv[0]);
        else
            printf("[DEBUG] empty command\n");

        cmds = cmds->next;
    }
}

void free_cmds(t_cmd *cmds)
{
    printf("[DEBUG] free_cmds called\n");

    while (cmds)
    {
        t_cmd *next = cmds->next;

        // later you will free argv, infile, outfile, etc
        free(cmds);

        cmds = next;
    }
}

void print_tokens(t_token *t)
{
    while (t)
    {
        if (t->type == TOK_WORD)
            printf("WORD(%s)\n", t->value);
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
